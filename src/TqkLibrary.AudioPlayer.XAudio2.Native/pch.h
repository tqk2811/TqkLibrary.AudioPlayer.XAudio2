#ifndef PCH_H
#define PCH_H

#ifdef TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVE_EXPORTS
#define TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS extern "C" __declspec( dllexport )
#else
#define TQKLIBRARYAUDIOPLAYERXAUDIO2NATIVEEXPORTS extern "C" __declspec( dllimport )
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>
#include <wrl/client.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include "libav.h"
using namespace Microsoft::WRL;

typedef class XAudio2Engine XAudio2Engine;
typedef class XAudio2MasterVoice XAudio2MasterVoice;
typedef class XAudio2SourceVoice XAudio2SourceVoice;
typedef class XAudio2VoiceCallback XAudio2VoiceCallback;
typedef class SwrConvert SwrConvert;

#include "XAudio2Engine.h"
#include "XAudio2MasterVoice.h"
#include "XAudio2SourceVoice.h"
#include "XAudio2VoiceCallback.h"
#include "SwrConvert.h"

#if _DEBUG
#include "DebugAudioSource.h"
#endif

#include "Exports.h"


#endif //PCH_H
