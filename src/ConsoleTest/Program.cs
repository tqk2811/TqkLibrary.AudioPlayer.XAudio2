// See https://aka.ms/new-console-template for more information
using TqkLibrary.AudioPlayer.XAudio2;

string filePath = ".\\01 Rainbow.mp3";
//string filePath = "D:\\test.mp4";
while (true)
{
    using DebugAudioSource debugAudioSource = new DebugAudioSource(filePath);
    using DebugAudioSource.AVFrame aVFrame = new DebugAudioSource.AVFrame();
    if (!debugAudioSource.ReadFrame(aVFrame))
    {
        throw new ApplicationException();
    }
    using XAudio2Engine engine = new XAudio2Engine();
    using XAudio2MasterVoice masterVoice = engine.CreateMasterVoice(2, 48000);
    using XAudio2SourceVoice sourceVoice = masterVoice.CreateSourceVoice(aVFrame.Handle);
    if (!sourceVoice.Start())
    {
        throw new ApplicationException();
    }
    if (!sourceVoice.SetChannelVolumes(1.0f, 1.0f))
    {
        throw new ApplicationException();
    }
    QueueResult queueResult;
    do
    {
        if (Console.KeyAvailable)
        {
            var keyInfo = Console.ReadKey();
            switch (keyInfo.Key)
            {
                case ConsoleKey.UpArrow:
                case ConsoleKey.DownArrow:
                    {
                        float volume = keyInfo.Key switch
                        {
                            ConsoleKey.UpArrow => 0.1f,
                            ConsoleKey.DownArrow => -0.1f,
                            _ => throw new NotSupportedException(),
                        };
                        volume += sourceVoice.Volume;
                        if (sourceVoice.SetVolume(volume))
                        {
                            Console.WriteLine($"Volume: {volume}");
                        }
                        else
                        {
                            throw new ApplicationException();
                        }
                        break;
                    }

                case ConsoleKey.LeftArrow:
                case ConsoleKey.RightArrow:
                    {
                        var volumes = sourceVoice.GetChannelVolumes();
                        if (volumes.Length >= 2)
                        {
                            switch (keyInfo.Key)
                            {
                                case ConsoleKey.LeftArrow:
                                    volumes[0] += 0.1f;
                                    volumes[volumes.Length - 1] -= 0.1f;
                                    break;
                                case ConsoleKey.RightArrow:
                                    volumes[0] -= 0.1f;
                                    volumes[volumes.Length - 1] += 0.1f;
                                    break;
                                default:
                                    throw new NotSupportedException();
                            }
                            if (sourceVoice.SetChannelVolumes(volumes))
                            {
                                Console.WriteLine($"ChannelVolumes: {string.Join(" : ", volumes)}");
                            }
                            else
                            {
                                throw new ApplicationException();
                            }
                        }
                        break;
                    }

                case ConsoleKey.R:
                    {
                        if (!sourceVoice.SetVolume(1.0f))
                        {
                            throw new ApplicationException();
                        }
                        float[] volumes = sourceVoice.GetChannelVolumes();
                        for (int i = 0; i < volumes.Length; i++) volumes[i] = 1.0f;
                        if (!sourceVoice.SetChannelVolumes(volumes))
                        {
                            throw new ApplicationException();
                        }
                        break;
                    }
            }
        }
        do
        {
            queueResult = sourceVoice.QueueFrame(aVFrame.Handle);
            switch (queueResult)
            {
                case QueueResult.Failed:
                    throw new ApplicationException();
                case QueueResult.QueueFull:
                    await Task.Delay(100);
                    continue;
            }
        }
        while (queueResult == QueueResult.QueueFull);
    }
    while (debugAudioSource.ReadFrame(aVFrame));
    queueResult = sourceVoice.QueueFrame(IntPtr.Zero, true);
}