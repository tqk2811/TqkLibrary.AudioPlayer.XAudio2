#include "pch.h"
#include "XAudio2Player.h"

XAudio2Player::XAudio2Player() {

}

XAudio2Player::~XAudio2Player() {

}

BOOL XAudio2Player::Init() {
	HRESULT hr;
	UINT32 flag = 0;
#if _DEBUG

#endif
	hr = XAudio2Create(this->_xaudio2.GetAddressOf(), flag);
	if (FAILED(hr))
		return false;

	hr = this->_xaudio2.Get()->StartEngine();
	if (FAILED(hr))
		return false;

	return SUCCEEDED(hr);
}

ComPtr<IXAudio2> XAudio2Player::GetInterface() {
	return this->_xaudio2;
}