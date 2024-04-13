#include "pch.h"
#include "XAudio2MasterVoice.h"

XAudio2MasterVoice::XAudio2MasterVoice(const XAudio2Engine* engine) {
	assert(engine);

	this->_xaudio2 = engine->_xaudio2;
	assert(this->_xaudio2);
}

XAudio2MasterVoice::~XAudio2MasterVoice() {
	if (this->_masterVoice)
	{
		this->_masterVoice->DestroyVoice();
		this->_masterVoice = nullptr;
	}
}

BOOL XAudio2MasterVoice::Init(int nb_channels, int sample_rate) {
	SetLastError(0);
	if (!this->_xaudio2)
		return FALSE;

	if (this->_masterVoice)
		return TRUE;

	HRESULT hr;
	hr = this->_xaudio2->CreateMasteringVoice(
		&this->_masterVoice,
		nb_channels,
		sample_rate
	); 
	SetLastError(hr);

	return SUCCEEDED(hr);
}

BOOL XAudio2MasterVoice::SetVolume(FLOAT volume) {
	SetLastError(0);
	if (!this->_masterVoice)
		return FALSE;

	HRESULT hr;
	hr = this->_masterVoice->SetVolume(volume);
	SetLastError(hr);
	return SUCCEEDED(hr);
}

VOID XAudio2MasterVoice::GetVolume(FLOAT* pVolume) {
	SetLastError(0);
	if (!this->_masterVoice)
		return;

	this->_masterVoice->GetVolume(pVolume);
}

BOOL XAudio2MasterVoice::SetChannelVolumes(UINT32 channels, const FLOAT* pVolume) {
	SetLastError(0);
	if (!this->_masterVoice)
		return FALSE;

	HRESULT hr;
	hr = this->_masterVoice->SetChannelVolumes(channels, pVolume);
	SetLastError(hr);
	return SUCCEEDED(hr);
}

VOID XAudio2MasterVoice::GetChannelVolumes(UINT32 channels, FLOAT* pVolume) {
	SetLastError(0);
	if (!this->_masterVoice)
		return;

	this->_masterVoice->GetChannelVolumes(channels, pVolume);
}