using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TqkLibrary.AudioPlayer.XAudio2
{
    public class XAudio2MasterVoice : IDisposable
    {
        readonly XAudio2Engine _engine;
        IntPtr _pointer = IntPtr.Zero;
        internal IntPtr Pointer { get { return _pointer; } }
        public float Volume
        {
            get
            {
                float volume = float.NaN;
                NativeWrapper.XAudio2MasterVoice_GetVolume(_pointer, ref volume);
                return volume;
            }
        }
        internal XAudio2MasterVoice(XAudio2Engine engine, IntPtr pAVFrame)
        {
            this._engine = engine ?? throw new ArgumentNullException(nameof(engine));
            _pointer = NativeWrapper.XAudio2MasterVoice_Alloc_AVFrame(engine.Pointer, pAVFrame);
            if (_pointer == IntPtr.Zero)
                throw new ApplicationException($"Create and load {nameof(XAudio2MasterVoice)} failed (last error : {NativeWrapper.GetLastError()})");
        }
        internal XAudio2MasterVoice(XAudio2Engine engine, int nb_channels, int sample_rate)
        {
            this._engine = engine ?? throw new ArgumentNullException(nameof(engine));
            _pointer = NativeWrapper.XAudio2MasterVoice_Alloc(engine.Pointer, nb_channels, sample_rate);
            if (_pointer == IntPtr.Zero)
                throw new ApplicationException($"Create and load {nameof(XAudio2MasterVoice)} failed (last error : {NativeWrapper.GetLastError()})");
        }
        ~XAudio2MasterVoice()
        {
            Dispose(false);
        }
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        void Dispose(bool disposing)
        {
            if (_pointer != IntPtr.Zero)
            {
                NativeWrapper.XAudio2MasterVoice_Free(ref _pointer);
            }
        }

        public bool SetVolume(float volume)
        {
            return NativeWrapper.XAudio2MasterVoice_SetVolume(_pointer, volume);
        }

        public bool SetChannelVolumes(float[] volumes)
        {
            return NativeWrapper.XAudio2MasterVoice_SetChannelVolumes(_pointer, (UInt32)volumes.Length, volumes);
        }
        public float[] GetChannelVolumes()
        {
            float[] volumes = Enumerable.Repeat<float>(-1.0f, 32).ToArray();
            NativeWrapper.XAudio2MasterVoice_GetChannelVolumes(_pointer, (UInt32)volumes.Length, ref volumes);
            return volumes.Where(x => x >= 0.0f).ToArray();
        }

        public XAudio2SourceVoice CreateSourceVoice(IntPtr pAVFrame)
        {
            return new XAudio2SourceVoice(this, pAVFrame);
        }
    }
}
