#ifndef _H_TqkLibraryAudioPlayerXAudio2Native_H_
#define _H_TqkLibraryAudioPlayerXAudio2Native_H_

TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS XAudio2Player* XAudio2Player_Alloc();
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS void XAudio2Player_Free(XAudio2Player** ppxAudio2Player);

TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS XAudio2Voice* XAudio2Voice_Alloc(XAudio2Player* pxAudio2Player, const AVFrame* pFrame);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS void XAudio2Voice_Free(XAudio2Voice** ppxAudio2Player);
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS BOOL XAudio2Voice_QueueFrame(XAudio2Voice* pXAudio2Voice, const AVFrame* pFrame);

#if _DEBUG
TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS void Test();
#endif
#endif // !ScrcpyNativeExports_H
