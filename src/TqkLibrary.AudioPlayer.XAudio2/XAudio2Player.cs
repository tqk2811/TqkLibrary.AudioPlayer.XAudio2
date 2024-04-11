using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace TqkLibrary.AudioPlayer.XAudio2
{
    public class XAudio2Player : IDisposable
    {
        IntPtr _pointer = IntPtr.Zero;
        internal IntPtr Pointer { get { return _pointer; } }
        public XAudio2Player()
        {
            _pointer = NativeWrapper.XAudio2Player_Alloc();
            if (_pointer == IntPtr.Zero)
                throw new ApplicationException($"Create and load IXAudio2 failed");
        }
        ~XAudio2Player()
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
                NativeWrapper.XAudio2Player_Free(ref _pointer);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="avFrame">FFmpeg AVFrame</param>
        /// <returns></returns>
        public XAudio2SourceVoice CreateXAudio2SourceVoice(IntPtr avFrame)
        {
            return new XAudio2SourceVoice(this, avFrame);
        }


#if DEBUG
        public static void Test()
        {
            NativeWrapper.Test();
        }
#endif
    }
}
