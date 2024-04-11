using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TqkLibrary.AudioPlayer.XAudio2
{
    internal static class NativeWrapper
    {
#if DEBUG
        static NativeWrapper()
        {
            string path = Path.Combine(
                Path.GetDirectoryName(Assembly.GetEntryAssembly()!.Location)!,
                "runtimes",
                Environment.Is64BitProcess ? "win-x64" : "win-x86",
                "native"
                );
            bool r = SetDllDirectory(path);
            if (!r)
                throw new InvalidOperationException("Can't set Kernel32.SetDllDirectory");
        }

        [DllImport("Kernel32.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool SetDllDirectory(string PathName);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Test();

#endif




        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr XAudio2Player_Alloc();

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2Player_Free(ref IntPtr intPtr);



        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr XAudio2SourceVoice_Alloc(IntPtr XAudio2_Pointer, IntPtr avFrame);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2SourceVoice_Free(ref IntPtr intPtr);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool XAudio2SourceVoice_QueueFrame(IntPtr handle, IntPtr avFrame);
    }
}
