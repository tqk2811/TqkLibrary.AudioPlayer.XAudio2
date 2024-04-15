using System.Runtime.InteropServices;

namespace TqkLibrary.AudioPlayer.XAudio2
{
    [StructLayout(LayoutKind.Sequential)]
    public struct XAUDIO2_VOICE_DETAILS
    {
        [MarshalAs(UnmanagedType.U4)]
        public UInt32 CreationFlags;

        [MarshalAs(UnmanagedType.U4)]
        public UInt32 ActiveFlags;

        [MarshalAs(UnmanagedType.U4)]
        public UInt32 InputChannels;

        [MarshalAs(UnmanagedType.U4)]
        public UInt32 InputSampleRate;
    }
}
