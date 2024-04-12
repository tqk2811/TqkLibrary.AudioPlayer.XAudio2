#include "pch.h"
#include "XAudio2VoiceCallback.h"


XAudio2VoiceCallback::XAudio2VoiceCallback() {
	this->_waitHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
	SetEvent(this->_waitHandle);
}

XAudio2VoiceCallback::~XAudio2VoiceCallback() {
	CloseHandle(this->_waitHandle);
}

void XAudio2VoiceCallback::OnVoiceProcessingPassStart(UINT32 BytesRequired) {

}
void XAudio2VoiceCallback::OnVoiceProcessingPassEnd() {

}
void XAudio2VoiceCallback::OnStreamEnd() {
	//SetEvent(this->_waitHandle);
}
void XAudio2VoiceCallback::OnBufferStart(void* pBufferContext) {

}
void XAudio2VoiceCallback::OnBufferEnd(void* pBufferContext) {
	SetEvent(this->_waitHandle);
	AVFrame* frame = (AVFrame*)pBufferContext;
	av_frame_free(&frame);
}
void XAudio2VoiceCallback::OnLoopEnd(void* pBufferContext) {

}
void XAudio2VoiceCallback::OnVoiceError(void* pBufferContext, HRESULT Error) {
	SetEvent(this->_waitHandle);
	AVFrame* frame = (AVFrame*)pBufferContext;
	av_frame_free(&frame);
}

BOOL XAudio2VoiceCallback::WaitReadEnd(DWORD timeout) {
	auto ret = WaitForSingleObject(this->_waitHandle, timeout);
	return ret == WAIT_OBJECT_0;
}
void XAudio2VoiceCallback::Reset() {
	ResetEvent(this->_waitHandle);
}