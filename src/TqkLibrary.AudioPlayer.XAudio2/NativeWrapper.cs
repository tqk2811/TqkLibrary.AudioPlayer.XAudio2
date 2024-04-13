using System;
using System.Collections.Generic;
using System.Drawing;
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
        internal static extern IntPtr DebugAudioSource_Alloc([MarshalAs(UnmanagedType.LPStr)] string filePath);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void DebugAudioSource_Free(ref IntPtr ppAudioSource);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool DebugAudioSource_ReadFrame(IntPtr pAudioSource, IntPtr pframe);


        [DllImport("avutil-57.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr av_frame_alloc();
        [DllImport("avutil-57.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void av_frame_free(ref IntPtr pAVFrame);



#endif

        [DllImport("Kernel32.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int GetLastError();



        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr XAudio2Engine_Alloc();

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2Engine_Free(ref IntPtr intPtr);




        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr XAudio2MasterVoice_Alloc(IntPtr pEngine, int nb_channels, int sample_rate);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr XAudio2MasterVoice_Alloc_AVFrame(IntPtr pEngine, IntPtr pAVFrame);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2MasterVoice_Free(ref IntPtr pMasterVoice);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool XAudio2MasterVoice_SetVolume(IntPtr pMasterVoice, float volume);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2MasterVoice_GetVolume(IntPtr pMasterVoice, ref float volume);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool XAudio2MasterVoice_SetChannelVolumes(IntPtr pMasterVoice, UInt32 channels, [MarshalAs(UnmanagedType.LPArray)] float[] volumes);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2MasterVoice_GetChannelVolumes(IntPtr pMasterVoice, UInt32 channels, [MarshalAs(UnmanagedType.LPArray)] ref float[] volumes);




        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr XAudio2SourceVoice_Alloc(IntPtr pMasterVoice, IntPtr pAVFrame);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2SourceVoice_Free(ref IntPtr pSourceVoice);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool XAudio2SourceVoice_Start(IntPtr pSourceVoice);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool XAudio2SourceVoice_Stop(IntPtr pSourceVoice, StopFlag stopFlag);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool XAudio2SourceVoice_SetVolume(IntPtr pSourceVoice, float volume);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2SourceVoice_GetVolume(IntPtr pSourceVoice, ref float volume);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool XAudio2SourceVoice_SetChannelVolumes(IntPtr pSourceVoice, UInt32 channels, [MarshalAs(UnmanagedType.LPArray)] float[] volumes);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2SourceVoice_GetChannelVolumes(IntPtr pSourceVoice, UInt32 channels, [MarshalAs(UnmanagedType.LPArray)] ref float[] volumes);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool XAudio2SourceVoice_QueueFrame(IntPtr pSourceVoice, IntPtr pAVFrame, bool isEof);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool XAudio2SourceVoice_FlushSourceBuffers(IntPtr pSourceVoice);
    }
}
