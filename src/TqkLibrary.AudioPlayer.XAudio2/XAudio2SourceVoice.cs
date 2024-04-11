namespace TqkLibrary.AudioPlayer.XAudio2
{
    public class XAudio2SourceVoice : IDisposable
    {
        readonly XAudio2Player _xAudio2Player;
        IntPtr _pointer = IntPtr.Zero;
        internal XAudio2SourceVoice(XAudio2Player xAudio2Player, IntPtr avFrame)
        {
            _xAudio2Player = xAudio2Player ?? throw new ArgumentNullException(nameof(xAudio2Player));
            _pointer = NativeWrapper.XAudio2SourceVoice_Alloc(xAudio2Player.Pointer, avFrame);
            if (_pointer == IntPtr.Zero)
                throw new ApplicationException($"Create and load IXAudio2 failed");
        }
        ~XAudio2SourceVoice()
        {
            Dispose(false);
        }
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        void Dispose(bool isDisposing)
        {
            if (_pointer != IntPtr.Zero)
                NativeWrapper.XAudio2SourceVoice_Free(ref _pointer);
        }

        public bool QueueFrame(IntPtr avFrame)
        {
            return NativeWrapper.XAudio2SourceVoice_QueueFrame(_pointer, avFrame);
        }
    }
}
