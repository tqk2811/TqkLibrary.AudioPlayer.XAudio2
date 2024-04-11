#include "pch.h"
#include "XAudio2VoiceCallback.h"
#include "XAudio2Voice.h"


XAudio2Voice::XAudio2Voice(ComPtr<IXAudio2> xaudio2) {
	assert(xaudio2);
	this->_xaudio2 = xaudio2;
	this->_callback = new XAudio2VoiceCallback();
}

XAudio2Voice::~XAudio2Voice() {
	if (_sourceVoice != nullptr) {
		_sourceVoice->DestroyVoice();
		_sourceVoice = nullptr;
	}
	if (this->_callback) {
		delete this->_callback;
		this->_callback = nullptr;
	}
	if (this->_swrConvert) {
		delete this->_swrConvert;
		this->_swrConvert = nullptr;
	}
	if (this->_tmpFrame) {
		av_frame_free(&this->_tmpFrame);
	}
}


BOOL XAudio2Voice::Init(const AVFrame* pFrame) {
	if (!pFrame)
		return FALSE;
	if (this->_sourceVoice || this->_masterVoice || this->_tmpFrame)
		return TRUE;

	WAVEFORMATEX sourceFormat;
	ZeroMemory(&sourceFormat, sizeof(WAVEFORMATEX));
	switch ((AVSampleFormat)pFrame->format)
	{
	case AVSampleFormat::AV_SAMPLE_FMT_U8:
		sourceFormat.wBitsPerSample = 8;
		sourceFormat.wFormatTag = WAVE_FORMAT_PCM;
		break;

	case AVSampleFormat::AV_SAMPLE_FMT_S16:
		sourceFormat.wBitsPerSample = 16;
		sourceFormat.wFormatTag = WAVE_FORMAT_PCM;
		break;

	case AVSampleFormat::AV_SAMPLE_FMT_S32:
		sourceFormat.wBitsPerSample = 32;
		sourceFormat.wFormatTag = WAVE_FORMAT_PCM;
		break;

	case AVSampleFormat::AV_SAMPLE_FMT_FLT:
		sourceFormat.wBitsPerSample = 32;
		sourceFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
		break;

	case AVSampleFormat::AV_SAMPLE_FMT_DBL:
		sourceFormat.wBitsPerSample = 64;
		sourceFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
		break;

	default:
	{
		switch ((AVSampleFormat)pFrame->format)
		{
		case AVSampleFormat::AV_SAMPLE_FMT_U8P:
			_outFormat = AVSampleFormat::AV_SAMPLE_FMT_U8;
			sourceFormat.wBitsPerSample = 8;
			sourceFormat.wFormatTag = WAVE_FORMAT_PCM;
			break;
		case AVSampleFormat::AV_SAMPLE_FMT_S16P:
			_outFormat = AVSampleFormat::AV_SAMPLE_FMT_S16;
			sourceFormat.wBitsPerSample = 16;
			sourceFormat.wFormatTag = WAVE_FORMAT_PCM;
			break;
		case AVSampleFormat::AV_SAMPLE_FMT_S32P:
			_outFormat = AVSampleFormat::AV_SAMPLE_FMT_S32;
			sourceFormat.wBitsPerSample = 32;
			sourceFormat.wFormatTag = WAVE_FORMAT_PCM;
			break;
		case AVSampleFormat::AV_SAMPLE_FMT_FLTP:
			_outFormat = AVSampleFormat::AV_SAMPLE_FMT_FLT;
			sourceFormat.wBitsPerSample = 32;
			sourceFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
			break;
		case AVSampleFormat::AV_SAMPLE_FMT_DBLP:
		case AVSampleFormat::AV_SAMPLE_FMT_S64:
		case AVSampleFormat::AV_SAMPLE_FMT_S64P:
			_outFormat = AVSampleFormat::AV_SAMPLE_FMT_DBL;
			sourceFormat.wBitsPerSample = 64;
			sourceFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
			break;

		default:
			return FALSE;
		}

		this->_swrConvert = new SwrConvert();
		if (!this->_swrConvert->Init(
			(AVChannelLayout*)&pFrame->ch_layout, _outFormat, pFrame->sample_rate,
			(AVChannelLayout*)&pFrame->ch_layout, (AVSampleFormat)pFrame->format, pFrame->sample_rate
		))
		{
			return FALSE;
		}
		this->_tmpFrame = av_frame_alloc();
		assert(this->_tmpFrame);
	}
	}

	HRESULT hr;
	hr = this->_xaudio2->CreateMasteringVoice(
		&this->_masterVoice,
		pFrame->ch_layout.nb_channels,
		pFrame->sample_rate,
		0
	);
	if (FAILED(hr))
		return FALSE;


	sourceFormat.nChannels = pFrame->ch_layout.nb_channels;
	sourceFormat.nSamplesPerSec = pFrame->sample_rate;
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

	hr = this->_xaudio2.Get()->CreateSourceVoice(
		&_sourceVoice,
		(WAVEFORMATEX*)&sourceFormat,
		flag,
		MaxFrequencyRatio,
		this->_callback,
		&sends,
		nullptr
	);
	if (FAILED(hr))
		return FALSE;

	hr = _sourceVoice->Start();
	if (FAILED(hr))
		return FALSE;

	return SUCCEEDED(hr);
}

BOOL XAudio2Voice::PlayFrame(const AVFrame* pFrame) {
	if (!pFrame)
		return FALSE;
	if (!_sourceVoice)
		return FALSE;

	HRESULT hr;

	XAUDIO2_BUFFER buffer{ 0 };
	buffer.Flags = 0;//or XAUDIO2_END_OF_STREAM
	if (this->_swrConvert)
	{
		av_frame_unref(this->_tmpFrame);
		this->_tmpFrame->ch_layout = pFrame->ch_layout;
		this->_tmpFrame->format = this->_outFormat;
		this->_tmpFrame->sample_rate = pFrame->sample_rate;
		if (this->_swrConvert->Convert(pFrame, this->_tmpFrame))
		{
			//buffer.AudioBytes = this->_tmpFrame->linesize[0];
			buffer.AudioBytes = min(this->_tmpFrame->linesize[0], pFrame->linesize[0]);
			//buffer.AudioBytes = this->_tmpFrame->nb_samples * this->_tmpFrame->ch_layout.nb_channels * 32 / 8;
			buffer.pAudioData = this->_tmpFrame->data[0];
		}
		else {
			return FALSE;
		}
	}
	else
	{
		buffer.AudioBytes = pFrame->linesize[0];
		buffer.pAudioData = pFrame->data[0];
	}


	this->_callback->Reset();
	do
	{
		hr = _sourceVoice->SubmitSourceBuffer(&buffer, nullptr);
		if (FAILED(hr))
		{
			if (hr == XAUDIO2_E_INVALID_CALL)
			{
				Sleep(10);
			}
			else return FALSE;
		}
		if (this->_callback->WaitReadEnd(INFINITE))
		{
			//wait success
			break;
		}
	} while (hr == XAUDIO2_E_INVALID_CALL);
#if _DEBUG
	if (FAILED(hr))
	{
		return SUCCEEDED(hr);
	}
#endif
	return SUCCEEDED(hr);
}