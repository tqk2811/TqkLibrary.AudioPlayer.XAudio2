#ifndef _H_TqkLibraryAudioPlayerXAudio2Native_H_
#define _H_TqkLibraryAudioPlayerXAudio2Native_H_

struct AudioDeviceInfo {
	WCHAR szDeviceId[256];
	WCHAR szDeviceName[256];
};

TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS int XAudio2_GetAudioDeviceCount();
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS BOOL XAudio2_GetAudioDeviceInfo(int index, AudioDeviceInfo* info);

TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS XAudio2Engine* XAudio2Engine_Alloc();
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS void XAudio2Engine_Free(XAudio2Engine** ppEngine);

TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS XAudio2MasterVoice* XAudio2MasterVoice_Alloc(const XAudio2Engine* pEngine, LPCWSTR szDeviceId, int nb_channels, int sample_rate);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS void XAudio2MasterVoice_Free(XAudio2MasterVoice** ppMasterVoice);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS BOOL XAudio2MasterVoice_SetVolume(XAudio2MasterVoice* pMasterVoice, FLOAT volume);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS VOID XAudio2MasterVoice_GetVolume(XAudio2MasterVoice* pMasterVoice, FLOAT* pVolume);

TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS XAudio2SourceVoice* XAudio2SourceVoice_Alloc(const XAudio2MasterVoice* pMasterVoice, int channels, int sampleRate, int bitsPerSample, WORD wFormatTag);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS void XAudio2SourceVoice_Free(XAudio2SourceVoice** ppSourceVoice);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS BOOL XAudio2SourceVoice_Start(XAudio2SourceVoice* pSourceVoice);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS BOOL XAudio2SourceVoice_Stop(XAudio2SourceVoice* pSourceVoice, UINT32 flag = 0);//0 or XAUDIO2_PLAY_TAILS
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS BOOL XAudio2SourceVoice_SetVolume(XAudio2SourceVoice* pSourceVoice, FLOAT volume);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS VOID XAudio2SourceVoice_GetVolume(XAudio2SourceVoice* pSourceVoice, FLOAT* volume);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS BOOL XAudio2SourceVoice_SetChannelVolumes(XAudio2SourceVoice* pSourceVoice, UINT32 channels, const FLOAT* pVolume);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS VOID XAudio2SourceVoice_GetChannelVolumes(XAudio2SourceVoice* pSourceVoice, UINT32 channels, FLOAT* pVolume);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS VOID XAudio2SourceVoice_GetVoiceDetails(XAudio2SourceVoice* pSourceVoice, XAUDIO2_VOICE_DETAILS* pVoiceDetails);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS VOID XAudio2SourceVoice_GetState(XAudio2SourceVoice* pSourceVoice, XAUDIO2_VOICE_STATE* pState, UINT32 flag);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS XAudio2SourceQueueResult XAudio2SourceVoice_QueueFrame(XAudio2SourceVoice* pSourceVoice, const BYTE* pData, UINT32 dataLength, BOOL isEof);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS BOOL XAudio2SourceVoice_FlushSourceBuffers(XAudio2SourceVoice* pSourceVoice);

#endif // !ScrcpyNativeExports_H
