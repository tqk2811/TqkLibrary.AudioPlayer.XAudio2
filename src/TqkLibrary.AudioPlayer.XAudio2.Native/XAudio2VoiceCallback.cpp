#include "pch.h"
#include "XAudio2VoiceCallback.h"


XAudio2VoiceCallback::XAudio2VoiceCallback() {
}

XAudio2VoiceCallback::~XAudio2VoiceCallback() {
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
	AVFrame* frame = (AVFrame*)pBufferContext;
	av_frame_free(&frame);
}
void XAudio2VoiceCallback::OnLoopEnd(void* pBufferContext) {

}
void XAudio2VoiceCallback::OnVoiceError(void* pBufferContext, HRESULT Error) {
	AVFrame* frame = (AVFrame*)pBufferContext;
	av_frame_free(&frame);
}