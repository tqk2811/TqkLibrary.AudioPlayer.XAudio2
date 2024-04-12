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
}


BOOL XAudio2Voice::Init(const AVFrame* pFrame) {
	if (!pFrame)
		return FALSE;
	if (this->_sourceVoice || this->_masterVoice)
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
	case AVSampleFormat::AV_SAMPLE_FMT_S64:
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
		case AVSampleFormat::AV_SAMPLE_FMT_S64P:
			_outFormat = AVSampleFormat::AV_SAMPLE_FMT_DBL;
			sourceFormat.wBitsPerSample = 64;
			sourceFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
			break;

		default:
			return FALSE;
		}

		AVChannelLayout outChlayout;
		outChlayout.nb_channels = pFrame->ch_layout.nb_channels;
		outChlayout.order = pFrame->ch_layout.order;
		if (pFrame->ch_layout.order == AV_CHANNEL_ORDER_NATIVE)
			outChlayout.u.mask = pFrame->ch_layout.u.mask;

		this->_swrConvert = new SwrConvert();
		if (!this->_swrConvert->Init(
			(AVChannelLayout*)&outChlayout, _outFormat, pFrame->sample_rate,
			(AVChannelLayout*)&pFrame->ch_layout, (AVSampleFormat)pFrame->format, pFrame->sample_rate
		))
		{
			return FALSE;
		}
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

	hr = this->_xaudio2->CreateSourceVoice(
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

BOOL XAudio2Voice::QueueFrame(const AVFrame* pFrame, BOOL isEof) {
	if (!_sourceVoice)
		return FALSE;

	HRESULT hr;
	AVFrame* newFrame{ nullptr };
	XAUDIO2_BUFFER buffer{ 0 };
	if (isEof)
	{
		buffer.Flags = XAUDIO2_END_OF_STREAM;//or 
	}
	else
	{
		buffer.Flags = 0;
		if (!pFrame)
			return FALSE;

		AVFrame* newFrame = av_frame_alloc();
		buffer.pContext = newFrame;
		if (this->_swrConvert)
		{
			newFrame->ch_layout.nb_channels = pFrame->ch_layout.nb_channels;
			newFrame->ch_layout.order = pFrame->ch_layout.order;
			newFrame->ch_layout.opaque = nullptr;
			if (pFrame->ch_layout.order == AV_CHANNEL_ORDER_NATIVE)
				newFrame->ch_layout.u.mask = pFrame->ch_layout.u.mask;
			newFrame->format = this->_outFormat;
			newFrame->sample_rate = pFrame->sample_rate;
			if (this->_swrConvert->Convert(pFrame, newFrame))
			{
				buffer.AudioBytes = av_samples_get_buffer_size(
					newFrame->linesize,
					newFrame->ch_layout.nb_channels,
					newFrame->nb_samples,
					(AVSampleFormat)newFrame->format,
					0
				);
				buffer.pAudioData = newFrame->data[0];
			}
			else {
				av_frame_free(&newFrame);
				return FALSE;
			}
		}
		else
		{
			av_frame_copy_props(newFrame, pFrame);
			av_frame_ref(newFrame, pFrame);
			buffer.AudioBytes = av_samples_get_buffer_size(
				newFrame->linesize,
				newFrame->ch_layout.nb_channels,
				newFrame->nb_samples,
				(AVSampleFormat)newFrame->format,
				0
			);
			buffer.pAudioData = newFrame->data[0];
		}
	}
	do
	{
		hr = _sourceVoice->SubmitSourceBuffer(&buffer, nullptr);
		if (FAILED(hr))
		{
			if (hr == XAUDIO2_E_INVALID_CALL && buffer.Flags != XAUDIO2_END_OF_STREAM)
			{
				Sleep(1);
			}
			else
			{
				/*if (tmpBuff)
				{
					delete[] tmpBuff;
					tmpBuff = nullptr;
				}*/
				av_frame_free(&newFrame);
				return FALSE;
			}
		}
	} while (hr == XAUDIO2_E_INVALID_CALL);
	return SUCCEEDED(hr);
}