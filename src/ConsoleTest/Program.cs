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
        return;
    }
    using XAudio2Engine engine = new XAudio2Engine();
    using XAudio2MasterVoice masterVoice = engine.CreateMasterVoice(aVFrame.Handle);
    using XAudio2SourceVoice sourceVoice = masterVoice.CreateSourceVoice(aVFrame.Handle);
    if (!sourceVoice.Start())
    {
        return;
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
                    {
                        float volume = sourceVoice.Volume + 0.1f;
                        sourceVoice.SetVolume(volume);
                        Console.WriteLine($"Volume: {volume}");
                        break;
                    }

                case ConsoleKey.DownArrow:
                    {
                        float volume = sourceVoice.Volume - 0.1f;
                        sourceVoice.SetVolume(volume);
                        Console.WriteLine($"Volume: {volume}");
                        break;
                    }
            }
        }
        while (true)
        {
            queueResult = sourceVoice.QueueFrame(aVFrame.Handle);
            if (queueResult == QueueResult.QueueFull)
            {
                await Task.Delay(100);
                continue;
            }
            else if (queueResult == QueueResult.Success)
            {
                break;
            }
            else
            {
                return;
            }
        }
    }
    while (debugAudioSource.ReadFrame(aVFrame));
    queueResult = sourceVoice.QueueFrame(IntPtr.Zero, true);
}
Console.WriteLine("End");

Console.ReadLine();