#ifndef _XAudio2Voice_H_
#define _XAudio2Voice_H_
class XAudio2Voice
{
public:
	XAudio2Voice(ComPtr<IXAudio2> _xaudio2);
	~XAudio2Voice();

	BOOL Init(const AVFrame* pFrame);
	BOOL PlayFrame(const AVFrame* pFrame);
private:
	ComPtr<IXAudio2> _xaudio2{ nullptr };
	IXAudio2MasteringVoice* _masterVoice{ nullptr };
	IXAudio2SourceVoice* _sourceVoice{ nullptr };
	XAudio2VoiceCallback* _callback{ nullptr };
	SwrConvert* _swrConvert{ nullptr };
	AVFrame* _tmpFrame{ nullptr };
	AVSampleFormat _outFormat{ AV_SAMPLE_FMT_NONE };
};
#endif
