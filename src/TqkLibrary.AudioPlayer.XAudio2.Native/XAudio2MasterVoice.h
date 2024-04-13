#ifndef _H_XAudio2MasterVoice_H_
#define _H_XAudio2MasterVoice_H_
class XAudio2MasterVoice
{
	friend XAudio2SourceVoice;
public:
	XAudio2MasterVoice(const XAudio2Engine* engine);
	~XAudio2MasterVoice();

	BOOL Init(int nb_channels, int sample_rate);

	BOOL SetVolume(FLOAT volume);
	VOID GetVolume(FLOAT* pVolume);

	BOOL SetChannelVolumes(UINT32 channels, const FLOAT* pVolume);
	VOID GetChannelVolumes(UINT32 channels, FLOAT* pVolume);

private:
	ComPtr<IXAudio2> _xaudio2{ nullptr };
	IXAudio2MasteringVoice* _masterVoice{ nullptr };
};
#endif

