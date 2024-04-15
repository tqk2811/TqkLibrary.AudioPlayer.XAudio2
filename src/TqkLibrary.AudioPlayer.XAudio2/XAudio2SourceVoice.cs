namespace TqkLibrary.AudioPlayer.XAudio2
{
    public class XAudio2SourceVoice : IDisposable
    {
        readonly XAudio2MasterVoice _masterVoice;
        IntPtr _pointer = IntPtr.Zero;

        public float Volume
        {
            get
            {
                float volume = float.NaN;
                NativeWrapper.XAudio2SourceVoice_GetVolume(_pointer, ref volume);
                return volume;
            }
        }
        internal XAudio2SourceVoice(XAudio2MasterVoice masterVoice, IntPtr pAVFrame)
        {
            _masterVoice = masterVoice ?? throw new ArgumentNullException(nameof(masterVoice));
            _pointer = NativeWrapper.XAudio2SourceVoice_Alloc(masterVoice.Pointer, pAVFrame);
            if (_pointer == IntPtr.Zero)
                throw new ApplicationException($"Create and load {nameof(XAudio2SourceVoice)} failed (last error : {NativeWrapper.GetLastError()})");
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

        public bool Start()
        {
            return NativeWrapper.XAudio2SourceVoice_Start(_pointer);
        }
        public bool Stop(StopFlag stopFlag = StopFlag.None)
        {
            return NativeWrapper.XAudio2SourceVoice_Stop(_pointer, stopFlag);
        }
        public bool SetVolume(float volume)
        {
            return NativeWrapper.XAudio2SourceVoice_SetVolume(_pointer, volume);
        }
        public bool SetChannelVolumes(float[] volumes)
        {
            return NativeWrapper.XAudio2SourceVoice_SetChannelVolumes(_pointer, (UInt32)volumes.Length, volumes);
        }
        public float[] GetChannelVolumes()
        {
            float[] volumes = Enumerable.Repeat<float>(-1.0f, 32).ToArray();
            NativeWrapper.XAudio2SourceVoice_GetChannelVolumes(_pointer, (UInt32)volumes.Length, ref volumes);
            return volumes.Where(x => x >= 0.0f).ToArray();
        }


        public QueueResult QueueFrame(IntPtr avFrame, bool isEof = false)
        {
            return NativeWrapper.XAudio2SourceVoice_QueueFrame(_pointer, avFrame, isEof);
        }

        /// <summary>
        /// Removes all pending audio buffers from the voice queue.
        /// </summary>
        /// <returns></returns>
        public bool FlushSourceBuffers()
        {
            return NativeWrapper.XAudio2SourceVoice_FlushSourceBuffers(_pointer);
        }
    }
}
