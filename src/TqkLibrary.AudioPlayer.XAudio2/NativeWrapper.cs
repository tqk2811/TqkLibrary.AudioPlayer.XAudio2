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
#if NETFRAMEWORK
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

        [DllImport("Kernel32.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi)]
        internal static extern bool SetDllDirectory(string PathName);
#endif
#endif

        [DllImport("Kernel32.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Winapi)]
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
        internal static extern void XAudio2SourceVoice_GetChannelVolumes(IntPtr pSourceVoice, UInt32 channels, IntPtr pVolumes);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2SourceVoice_GetVoiceDetails(IntPtr pSourceVoice, ref XAUDIO2_VOICE_DETAILS voiceDetails);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XAudio2SourceVoice_GetState(IntPtr pSourceVoice, ref XAUDIO2_VOICE_STATE state, UInt32 flag);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern QueueResult XAudio2SourceVoice_QueueFrame(IntPtr pSourceVoice, IntPtr pAVFrame, bool isEof);

        [DllImport("TqkLibrary.AudioPlayer.XAudio2.Native.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool XAudio2SourceVoice_FlushSourceBuffers(IntPtr pSourceVoice);
    }
}
