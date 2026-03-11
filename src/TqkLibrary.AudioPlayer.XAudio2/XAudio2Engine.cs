using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using TqkLibrary.AudioPlayer.XAudio2.Structs;

namespace TqkLibrary.AudioPlayer.XAudio2
{
    public class XAudio2Engine : IDisposable
    {
        IntPtr _pointer = IntPtr.Zero;
        internal IntPtr Pointer { get { return _pointer; } }
        public XAudio2Engine()
        {
            _pointer = NativeWrapper.XAudio2Engine_Alloc();
            if (_pointer == IntPtr.Zero)
                throw new ApplicationException($"Create and load {nameof(XAudio2Engine)} failed (last error : {NativeWrapper.GetLastError()})");
        }
        ~XAudio2Engine()
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
                NativeWrapper.XAudio2Engine_Free(ref _pointer);
        }

        /// <summary>
        /// Create a master voice with specified audio format
        /// </summary>
        /// <param name="nb_channels">Number of channels</param>
        /// <param name="sample_rate">Sample rate in Hz</param>
        /// <returns></returns>
        public XAudio2MasterVoice CreateMasterVoice(int nb_channels, int sample_rate, string? deviceId = null)
        {
            return new XAudio2MasterVoice(this, nb_channels, sample_rate, deviceId);
        }

        public static IEnumerable<AudioDeviceInfo> GetAudioDevices()
        {
            int count = NativeWrapper.XAudio2_GetAudioDeviceCount();
            for (int i = 0; i < count; i++)
            {
                var info = new AudioDeviceInfo();
                if (NativeWrapper.XAudio2_GetAudioDeviceInfo(i, ref info))
                {
                    yield return info;
                }
            }
        }
    }
}
