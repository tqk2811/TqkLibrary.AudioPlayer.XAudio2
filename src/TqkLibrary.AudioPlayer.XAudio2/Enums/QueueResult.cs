namespace TqkLibrary.AudioPlayer.XAudio2
{
    public enum QueueResult : int
    {
        Failed = -1,
        Success = 0,
        /// <summary>
        /// Must delay and queue again later
        /// </summary>
        QueueFull = 1,
    }
}
