#ifndef _XAudio2Engine_H_
#define _XAudio2Engine_H_
class XAudio2Engine
{
	friend XAudio2MasterVoice;
public:
	XAudio2Engine();
	~XAudio2Engine();
	BOOL Init();


private:
	ComPtr<IXAudio2> _xaudio2{ nullptr };
};
#endif // !_XAudio2Engine_H_


