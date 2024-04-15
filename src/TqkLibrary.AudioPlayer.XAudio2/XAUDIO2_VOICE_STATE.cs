using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TqkLibrary.AudioPlayer.XAudio2
{
    [StructLayout(LayoutKind.Sequential)]
    public struct XAUDIO2_VOICE_STATE
    {
        public IntPtr pCurrentBufferContext;

        [MarshalAs(UnmanagedType.U4)]
        public UInt32 BuffersQueued;

        [MarshalAs(UnmanagedType.U8)]
        public long SamplesPlayed;
    }
}
