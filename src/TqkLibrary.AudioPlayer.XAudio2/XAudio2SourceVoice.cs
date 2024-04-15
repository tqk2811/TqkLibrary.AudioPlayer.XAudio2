using System;
using System.Runtime.InteropServices;

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
        public bool SetChannelVolumes(params float[] volumes)
        {
            return NativeWrapper.XAudio2SourceVoice_SetChannelVolumes(_pointer, (UInt32)volumes.Length, volumes);
        }
        public float[] GetChannelVolumes()
        {
            float[] volumes = new float[GetVoiceDetails().InputChannels];
            for (int i = 0; i < volumes.Length; i++) volumes[i] = float.NaN;
            GCHandle handle = GCHandle.Alloc(volumes, GCHandleType.Pinned);
            NativeWrapper.XAudio2SourceVoice_GetChannelVolumes(_pointer, (UInt32)volumes.Length, handle.AddrOfPinnedObject());
            handle.Free();
            return volumes;
        }

        public XAUDIO2_VOICE_DETAILS GetVoiceDetails()
        {
            XAUDIO2_VOICE_DETAILS voiceDetails = new XAUDIO2_VOICE_DETAILS();
            NativeWrapper.XAudio2SourceVoice_GetVoiceDetails(_pointer, ref voiceDetails);
            return voiceDetails;
        }

        /// <summary>
        /// Disable get <see cref="XAUDIO2_VOICE_STATE.SamplesPlayed"/>
        /// </summary>
        public const UInt32 XAUDIO2_VOICE_NOSAMPLESPLAYED = 0x0100;
        /// <summary>
        /// 
        /// </summary>
        /// <param name="flag">Defaut: <see cref="XAUDIO2_VOICE_NOSAMPLESPLAYED"/></param>
        /// <returns></returns>
        public XAUDIO2_VOICE_STATE GetState(UInt32 flag = XAUDIO2_VOICE_NOSAMPLESPLAYED)
        {
            XAUDIO2_VOICE_STATE state = new XAUDIO2_VOICE_STATE();
            NativeWrapper.XAudio2SourceVoice_GetState(_pointer, ref state, flag);
            return state;
        }

        /// <summary>
        /// Clone or Convert this frame and put into the queue
        /// </summary>
        /// <param name="avFrame">Note: must realse your frame after funtion</param>
        /// <param name="isEof"></param>
        /// <returns></returns>
        public QueueResult QueueFrame(IntPtr pAVFrame, bool isEof = false)
        {
            return NativeWrapper.XAudio2SourceVoice_QueueFrame(_pointer, pAVFrame, isEof);
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
