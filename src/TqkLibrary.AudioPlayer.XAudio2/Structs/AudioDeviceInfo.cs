using System.Runtime.InteropServices;

namespace TqkLibrary.AudioPlayer.XAudio2.Structs
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct AudioDeviceInfo
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string DeviceId;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string DeviceName;
    }
}
