using TqkLibrary.AudioPlayer.XAudio2;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

string filePath = "01 Rainbow.mp3";
if (!File.Exists(filePath))
{
    filePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "01 Rainbow.mp3");
}
#if NETFRAMEWORK
string ffmpegPath = AppDomain.CurrentDomain.BaseDirectory;
#else
string ffmpegPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "runtimes", Environment.Is64BitProcess ? "win-x64" : "win-x86", "native");
#endif
string ffmpegExe = Path.Combine(ffmpegPath, "ffmpeg.exe");
filePath = Path.GetFullPath(filePath);
if (!File.Exists(filePath))
{
    Console.Error.WriteLine($"Error: File not found: {filePath}");
    return;
}

Console.WriteLine($"Using ffmpeg from: {ffmpegExe}");
Console.WriteLine($"Opening file: {filePath}");

// We'll force ffmpeg to output specific PCM format
int channels = 2;
int sampleRate = 48000;
int bitsPerSample = 16; 

Console.WriteLine($"Target Info: {channels} channels, {sampleRate}Hz (forced via ffmpeg)");

using XAudio2Engine engine = new XAudio2Engine();
using XAudio2MasterVoice masterVoice = engine.CreateMasterVoice(channels, sampleRate);
using XAudio2SourceVoice sourceVoice = masterVoice.CreateSourceVoice(channels, sampleRate, bitsPerSample, false);

if (!sourceVoice.Start())
{
    throw new ApplicationException("Failed to start source voice");
}

Console.WriteLine("Playing... Press 'Q' to quit.");
Console.WriteLine("Up/Down: Volume, Left/Right: Channel Balance, R: Reset");

CancellationTokenSource cts = new CancellationTokenSource();

// Use a direct approach with Process to stream data block by block.
var processStartInfo = new ProcessStartInfo
{
    FileName = ffmpegExe,
    Arguments = $"-i \"{filePath}\" -f s16le -acodec pcm_s16le -ar {sampleRate} -ac {channels} pipe:1",
    RedirectStandardOutput = true,
    RedirectStandardError = true,
    UseShellExecute = false,
    CreateNoWindow = true
};

using var process = Process.Start(processStartInfo);
if (process == null) throw new ApplicationException("Failed to start ffmpeg process");

// Capture ffmpeg logs to Console.Error so the user can see them
process.ErrorDataReceived += (s, e) => { if (e.Data != null) Console.Error.WriteLine($"FFMpeg: {e.Data}"); };
process.BeginErrorReadLine();

byte[] buffer = new byte[sampleRate * channels * (bitsPerSample / 8) / 10]; // 100ms buffer

var readTask = Task.Run(async () =>
{
    try 
    {
        using var stdout = process.StandardOutput.BaseStream;
        while (!cts.Token.IsCancellationRequested)
        {
            int bytesToRead = buffer.Length;
            int totalRead = 0;
            while (totalRead < bytesToRead)
            {
                int read = await stdout.ReadAsync(buffer, totalRead, bytesToRead - totalRead, cts.Token);
                if (read == 0) 
                    break;
                totalRead += read;
            }

            if (totalRead == 0) 
                break;

            byte[] dataToQueue = new byte[totalRead];
            Array.Copy(buffer, 0, dataToQueue, 0, totalRead);

            QueueResult queueResult;
            do
            {
                queueResult = sourceVoice.QueueFrame(dataToQueue);
                if (queueResult == QueueResult.QueueFull)
                {
                    await Task.Delay(10, cts.Token);
                }
            } while (queueResult == QueueResult.QueueFull && !cts.Token.IsCancellationRequested);

            if (queueResult == QueueResult.Failed)
            {
                Console.WriteLine("QueueFrame Failed");
                break;
            }
        }
        
        sourceVoice.QueueFrame(Array.Empty<byte>(), true);
        Console.WriteLine("Playback finished.");
    }
    catch (OperationCanceledException) { }
    catch (Exception ex)
    {
        Console.WriteLine($"Error in playback: {ex.Message}");
    }
});

while (!readTask.IsCompleted)
{
    if (Console.KeyAvailable)
    {
        var keyInfo = Console.ReadKey(true);
        if (keyInfo.Key == ConsoleKey.Q)
        {
            cts.Cancel();
            try { process.Kill(); } catch { }
            break;
        }
        
        switch (keyInfo.Key)
        {
            case ConsoleKey.UpArrow:
            case ConsoleKey.DownArrow:
                float vol = sourceVoice.Volume + (keyInfo.Key == ConsoleKey.UpArrow ? 0.1f : -0.1f);
                if (sourceVoice.SetVolume(vol)) Console.WriteLine($"Volume: {vol:F1}");
                break;

            case ConsoleKey.LeftArrow:
            case ConsoleKey.RightArrow:
                var currentVolumes = sourceVoice.GetChannelVolumes();
                if (currentVolumes.Length >= 2)
                {
                    if (keyInfo.Key == ConsoleKey.LeftArrow) { currentVolumes[0] += 0.1f; currentVolumes[1] -= 0.1f; }
                    else { currentVolumes[0] -= 0.1f; currentVolumes[1] += 0.1f; }
                    if (sourceVoice.SetChannelVolumes(currentVolumes)) Console.WriteLine($"Balance: {currentVolumes[0]:F1} : {currentVolumes[1]:F1}");
                }
                break;

            case ConsoleKey.R:
                sourceVoice.SetVolume(1.0f);
                sourceVoice.SetChannelVolumes(1.0f, 1.0f);
                Console.WriteLine("Reset Audio Settings");
                break;
        }
    }
    await Task.Delay(100);
}

await readTask;