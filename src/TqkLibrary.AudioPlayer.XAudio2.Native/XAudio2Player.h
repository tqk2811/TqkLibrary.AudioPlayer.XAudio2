#ifndef _XAudio2Player_H_
#define _XAudio2Player_H_
class XAudio2Player
{
public:
	XAudio2Player();
	~XAudio2Player();
	BOOL Init();

	ComPtr<IXAudio2> GetInterface();

private:
	ComPtr<IXAudio2> _xaudio2{ nullptr };
};
#endif // !_XAudio2Player_H_


