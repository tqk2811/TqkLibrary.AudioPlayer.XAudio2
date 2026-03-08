using TqkLibrary.AudioPlayer.XAudio2;

// Simple sine wave generator for testing
const int sampleRate = 48000;
const int channels = 2;
const int bitsPerSample = 16;
const double frequency = 440.0; // A4 note
const int samplesPerBuffer = 4800; // 100ms per buffer

using XAudio2Engine engine = new XAudio2Engine();
using XAudio2MasterVoice masterVoice = engine.CreateMasterVoice(channels, sampleRate);
using XAudio2SourceVoice sourceVoice = masterVoice.CreateSourceVoice(channels, sampleRate, bitsPerSample, false);

if (!sourceVoice.Start())
{
    throw new ApplicationException("Failed to start source voice");
}

if (!sourceVoice.SetChannelVolumes(1.0f, 1.0f))
{
    throw new ApplicationException("Failed to set channel volumes");
}

Console.WriteLine("Playing sine wave... Press 'Q' to quit.");
Console.WriteLine("Up/Down: Volume, Left/Right: Channel Balance, R: Reset");

int sampleIndex = 0;

while (true)
{
    if (Console.KeyAvailable)
    {
        var keyInfo = Console.ReadKey(true);
        switch (keyInfo.Key)
        {
            case ConsoleKey.Q:
                // Send EOF
                sourceVoice.QueueFrame(Array.Empty<byte>(), true);
                return;

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
                    float[] cvols = sourceVoice.GetChannelVolumes();
                    for (int i = 0; i < cvols.Length; i++) cvols[i] = 1.0f;
                    if (!sourceVoice.SetChannelVolumes(cvols))
                    {
                        throw new ApplicationException();
                    }
                    Console.WriteLine("Reset volume and channel balance");
                    break;
                }
        }
    }

    // Generate a buffer of sine wave PCM data
    byte[] audioData = GenerateSineWave(ref sampleIndex, samplesPerBuffer, sampleRate, frequency, channels, bitsPerSample);

    QueueResult queueResult;
    do
    {
        queueResult = sourceVoice.QueueFrame(audioData);
        switch (queueResult)
        {
            case QueueResult.Failed:
                throw new ApplicationException("QueueFrame failed");
            case QueueResult.QueueFull:
                await Task.Delay(10);
                continue;
        }
    }
    while (queueResult == QueueResult.QueueFull);
}

static byte[] GenerateSineWave(ref int sampleIndex, int sampleCount, int sampleRate, double frequency, int channels, int bitsPerSample)
{
    int bytesPerSample = bitsPerSample / 8;
    byte[] buffer = new byte[sampleCount * channels * bytesPerSample];
    int offset = 0;

    for (int i = 0; i < sampleCount; i++)
    {
        double t = (double)sampleIndex / sampleRate;
        double value = Math.Sin(2.0 * Math.PI * frequency * t);
        sampleIndex++;

        for (int ch = 0; ch < channels; ch++)
        {
            switch (bitsPerSample)
            {
                case 16:
                    short sample16 = (short)(value * short.MaxValue);
                    BitConverter.GetBytes(sample16).CopyTo(buffer, offset);
                    break;
                case 32:
                    int sample32 = (int)(value * int.MaxValue);
                    BitConverter.GetBytes(sample32).CopyTo(buffer, offset);
                    break;
                default:
                    throw new NotSupportedException($"Unsupported bits per sample: {bitsPerSample}");
            }
            offset += bytesPerSample;
        }
    }

    return buffer;
}