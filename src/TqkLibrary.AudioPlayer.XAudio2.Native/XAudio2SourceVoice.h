#ifndef _XAudio2SourceVoice_H_
#define _XAudio2SourceVoice_H_
enum XAudio2SourceQueueResult : INT32
{
	XAudio2SourceQueue_Failed = -1,
	XAudio2SourceQueue_Success = 0,
	XAudio2SourceQueue_QueueFull = 1,
};

class XAudio2SourceVoice
{
public:
	XAudio2SourceVoice(const XAudio2MasterVoice* masterVoice);
	~XAudio2SourceVoice();

	BOOL Init(const AVFrame* pFrame);

	BOOL Start();
	BOOL Stop(UINT32 flag = 0);//0 or XAUDIO2_PLAY_TAILS

	BOOL SetVolume(FLOAT volume);
	VOID GetVolume(FLOAT* volume);

	BOOL SetChannelVolumes(UINT32 channels, const FLOAT* pVolume);
	VOID GetChannelVolumes(UINT32 channels, FLOAT* pVolume);


	XAudio2SourceQueueResult QueueFrame(const AVFrame* pFrame, BOOL isEof = FALSE);
	BOOL FlushSourceBuffers();


private:
	ComPtr<IXAudio2> _xaudio2{ nullptr };
	IXAudio2MasteringVoice* _masterVoice{ nullptr };
	IXAudio2SourceVoice* _sourceVoice{ nullptr };
	XAudio2VoiceCallback* _callback{ nullptr };
	SwrConvert* _swrConvert{ nullptr };
	AVSampleFormat _outFormat{ AV_SAMPLE_FMT_NONE };
};
#endif
