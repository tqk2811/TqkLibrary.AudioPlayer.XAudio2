#if _DEBUG
#ifndef _H_DebugAudioSource_H_
#define _H_DebugAudioSource_H_
class DebugAudioSource
{
public:
	DebugAudioSource();
	~DebugAudioSource();

	BOOL Init(LPSTR filePath);

	BOOL ReadFrame(AVFrame* pframe);

private:
	AVFormatContext* pInputFormatContext{ nullptr };
	AVCodecContext* pCodecContext{ nullptr };
	int audioIndex = -1;
	AVStream* input_audio_stream{ nullptr };
};
#endif
#endif
