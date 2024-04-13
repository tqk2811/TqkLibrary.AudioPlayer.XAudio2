#include "pch.h"
#include "XAudio2Engine.h"

XAudio2Engine::XAudio2Engine() {

}

XAudio2Engine::~XAudio2Engine() {

}

BOOL XAudio2Engine::Init() {
	HRESULT hr{ 0 };
	UINT32 flag = 0;
	SetLastError(hr);
#if _DEBUG

#endif
	hr = XAudio2Create(this->_xaudio2.GetAddressOf(), flag);
	SetLastError(hr);
	if (FAILED(hr))
		return FALSE;

	hr = this->_xaudio2->StartEngine();
	SetLastError(hr);

	return SUCCEEDED(hr);
}