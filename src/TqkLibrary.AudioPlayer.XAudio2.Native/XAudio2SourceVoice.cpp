#include "pch.h"
#include "XAudio2VoiceCallback.h"
#include "XAudio2SourceVoice.h"


XAudio2SourceVoice::XAudio2SourceVoice(const XAudio2MasterVoice* masterVoice) {
	assert(masterVoice);

	this->_xaudio2 = masterVoice->_xaudio2;
	this->_masterVoice = masterVoice->_masterVoice;
	this->_callback = new XAudio2VoiceCallback();
}

XAudio2SourceVoice::~XAudio2SourceVoice() {
	if (_sourceVoice != nullptr) {
		_sourceVoice->DestroyVoice();
		_sourceVoice = nullptr;
	}
	if (this->_callback) {
		delete this->_callback;
		this->_callback = nullptr;
	}
}


BOOL XAudio2SourceVoice::Init(int channels, int sampleRate, int bitsPerSample, BOOL isFloat) {
	SetLastError(0);
	if (channels <= 0 || sampleRate <= 0 || bitsPerSample <= 0)
		return FALSE;
	if (this->_sourceVoice)
		return TRUE;
	HRESULT hr{ 0 };
	WAVEFORMATEX sourceFormat;
	ZeroMemory(&sourceFormat, sizeof(WAVEFORMATEX));

	sourceFormat.wBitsPerSample = (WORD)bitsPerSample;
	sourceFormat.wFormatTag = isFloat ? WAVE_FORMAT_IEEE_FLOAT : WAVE_FORMAT_PCM;
	sourceFormat.nChannels = (WORD)channels;
	sourceFormat.nSamplesPerSec = (DWORD)sampleRate;
	sourceFormat.nBlockAlign = sourceFormat.nChannels * (sourceFormat.wBitsPerSample / 8);
	sourceFormat.nAvgBytesPerSec = sourceFormat.nSamplesPerSec * sourceFormat.nBlockAlign;
	sourceFormat.cbSize = 0;
	UINT32 flag = 0;
	FLOAT MaxFrequencyRatio = XAUDIO2_DEFAULT_FREQ_RATIO;
	XAUDIO2_SEND_DESCRIPTOR send_descriptor;
	send_descriptor.Flags = 0;
	send_descriptor.pOutputVoice = this->_masterVoice;
	XAUDIO2_VOICE_SENDS sends;
	sends.SendCount = 1;
	sends.pSends = &send_descriptor;

	hr = this->_xaudio2->CreateSourceVoice(
		&_sourceVoice,
		(WAVEFORMATEX*)&sourceFormat,
		flag,
		MaxFrequencyRatio,
		this->_callback,
		&sends,
		nullptr
	);
	SetLastError(hr);

	return SUCCEEDED(hr);
}
BOOL XAudio2SourceVoice::Start() {
	SetLastError(0);
	if (!this->_sourceVoice)
		return FALSE;

	HRESULT hr{ 0 };
	hr = _sourceVoice->Start();
	SetLastError(hr);

	return SUCCEEDED(hr);
}
BOOL XAudio2SourceVoice::Stop(UINT32 flag) {
	SetLastError(0);
	if (!this->_sourceVoice)
		return FALSE;

	HRESULT hr{ 0 };
	hr = _sourceVoice->Stop(flag);
	SetLastError(hr);
	return SUCCEEDED(hr);
}
BOOL XAudio2SourceVoice::SetVolume(FLOAT volume) {
	SetLastError(0);
	if (!this->_sourceVoice)
		return FALSE;

	HRESULT hr{ 0 };
	hr = _sourceVoice->SetVolume(volume);
	SetLastError(hr);

	return SUCCEEDED(hr);
}
VOID XAudio2SourceVoice::GetVolume(FLOAT* volume) {
	SetLastError(0);
	if (!this->_sourceVoice)
		return;
	_sourceVoice->GetVolume(volume);
}

BOOL XAudio2SourceVoice::SetChannelVolumes(UINT32 channels, const FLOAT* pVolume) {
	SetLastError(0);
	if (!this->_sourceVoice)
		return FALSE;

	HRESULT hr{ 0 };
	hr = _sourceVoice->SetChannelVolumes(channels, pVolume);
	SetLastError(hr);

	return SUCCEEDED(hr);
}
VOID XAudio2SourceVoice::GetChannelVolumes(UINT32 channels, FLOAT* pVolume) {
	SetLastError(0);
	if (!this->_sourceVoice)
		return;

	_sourceVoice->GetChannelVolumes(channels, pVolume);
}
VOID XAudio2SourceVoice::GetVoiceDetails(XAUDIO2_VOICE_DETAILS* pVoiceDetails) {
	SetLastError(0);
	if (!this->_sourceVoice)
		return;

	_sourceVoice->GetVoiceDetails(pVoiceDetails);
}
VOID XAudio2SourceVoice::GetState(XAUDIO2_VOICE_STATE* pState, UINT32 flag) {
	SetLastError(0);
	if (!this->_sourceVoice)
		return;

	_sourceVoice->GetState(pState, flag);
}

XAudio2SourceQueueResult XAudio2SourceVoice::QueueFrame(const BYTE* pData, UINT32 dataLength, BOOL isEof) {
	SetLastError(0);
	if (!_sourceVoice)
		return XAudio2SourceQueueResult::XAudio2SourceQueue_Failed;

	HRESULT hr;
	XAUDIO2_BUFFER buffer{ 0 };
	if (isEof)
	{
		buffer.Flags = XAUDIO2_END_OF_STREAM;
	}
	else
	{
		buffer.Flags = 0;
		if (!pData || dataLength == 0)
			return XAudio2SourceQueueResult::XAudio2SourceQueue_Failed;

		// Copy data to a new buffer that will be freed by the callback
		BYTE* pCopy = (BYTE*)malloc(dataLength);
		if (!pCopy)
			return XAudio2SourceQueueResult::XAudio2SourceQueue_Failed;

		memcpy(pCopy, pData, dataLength);
		buffer.pContext = pCopy;
		buffer.AudioBytes = dataLength;
		buffer.pAudioData = pCopy;
	}

	hr = _sourceVoice->SubmitSourceBuffer(&buffer, nullptr);
	if (SUCCEEDED(hr))
	{
		return XAudio2SourceQueueResult::XAudio2SourceQueue_Success;
	}
	else
	{
		if (buffer.pContext)
			free(buffer.pContext);
		SetLastError(hr);
		if (hr == XAUDIO2_E_INVALID_CALL)
		{
			XAUDIO2_VOICE_STATE state{ 0 };
			_sourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			if (state.BuffersQueued > 0)
			{
				return XAudio2SourceQueueResult::XAudio2SourceQueue_QueueFull;
			}
		}
		return XAudio2SourceQueueResult::XAudio2SourceQueue_Failed;
	}
}

BOOL XAudio2SourceVoice::FlushSourceBuffers() {
	SetLastError(0);
	if (!_sourceVoice)
		return FALSE;

	HRESULT hr{ 0 };
	hr = _sourceVoice->FlushSourceBuffers();
	SetLastError(hr);

	return SUCCEEDED(hr);
}