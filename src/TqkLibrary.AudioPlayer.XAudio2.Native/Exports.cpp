#include "pch.h"
#include "Exports.h"

XAudio2Engine* XAudio2Engine_Alloc() {
	SetLastError(0);
	XAudio2Engine* pEngine = new XAudio2Engine();
	if (pEngine->Init())
	{
		return pEngine;
	}
	else
	{
		delete pEngine;
		return nullptr;
	}
}
void XAudio2Engine_Free(XAudio2Engine** ppEngine) {
	if (ppEngine)
	{
		XAudio2Engine* pEngine = *ppEngine;
		if (pEngine)
		{
			delete pEngine;
			*ppEngine = nullptr;
		}
	}
}


XAudio2MasterVoice* XAudio2MasterVoice_Alloc(const XAudio2Engine* pEngine, int nb_channels, int sample_rate) {
	SetLastError(0);
	if (!pEngine)
		return nullptr;

	XAudio2MasterVoice* pMasterVocie = new XAudio2MasterVoice(pEngine);
	if (pMasterVocie->Init(nb_channels, sample_rate))
	{
		return pMasterVocie;
	}
	else
	{
		delete pMasterVocie;
		return nullptr;
	}
}
XAudio2MasterVoice* XAudio2MasterVoice_Alloc_AVFrame(const XAudio2Engine* pEngine, const AVFrame* pFrame) {
	SetLastError(0);
	if (!pEngine || !pFrame)
		return nullptr;

	return XAudio2MasterVoice_Alloc(pEngine, pFrame->ch_layout.nb_channels, pFrame->sample_rate);
}
void XAudio2MasterVoice_Free(XAudio2MasterVoice** ppMasterVoice)
{
	if (ppMasterVoice)
	{
		XAudio2MasterVoice* pMasterVoice = *ppMasterVoice;
		if (pMasterVoice)
		{
			delete pMasterVoice;
			*ppMasterVoice = nullptr;
		}
	}
}
BOOL XAudio2MasterVoice_SetVolume(XAudio2MasterVoice* pMasterVoice, FLOAT volume) {
	SetLastError(0);
	if (!pMasterVoice)
		return FALSE;
	return pMasterVoice->SetVolume(volume);
}
VOID XAudio2MasterVoice_GetVolume(XAudio2MasterVoice* pMasterVoice, FLOAT* pVolume) {
	SetLastError(0);
	if (!pMasterVoice)
		return;
	pMasterVoice->GetVolume(pVolume);
}
BOOL XAudio2MasterVoice_SetChannelVolumes(XAudio2MasterVoice* pMasterVoice, UINT32 channels, const FLOAT* pVolume) {
	SetLastError(0);
	if (!pMasterVoice)
		return FALSE;
	return pMasterVoice->SetChannelVolumes(channels, pVolume);
}
VOID XAudio2MasterVoice_GetChannelVolumes(XAudio2MasterVoice* pMasterVoice, UINT32 channels, FLOAT* pVolume) {
	SetLastError(0);
	if (!pMasterVoice)
		return;
	pMasterVoice->GetChannelVolumes(channels, pVolume);
}


XAudio2SourceVoice* XAudio2SourceVoice_Alloc(const XAudio2MasterVoice* pMasterVoice, const AVFrame* pFrame) {
	SetLastError(0);
	if (!pMasterVoice || !pFrame)
		return nullptr;

	XAudio2SourceVoice* pSourceVoice = new XAudio2SourceVoice(pMasterVoice);
	if (pSourceVoice->Init(pFrame))
	{
		return pSourceVoice;
	}
	else
	{
		delete pSourceVoice;
		return nullptr;
	}
}
void XAudio2SourceVoice_Free(XAudio2SourceVoice** ppSourceVoice) {
	if (ppSourceVoice)
	{
		XAudio2SourceVoice* pSourceVoice = *ppSourceVoice;
		if (pSourceVoice)
		{
			delete pSourceVoice;
			*ppSourceVoice = nullptr;
		}
	}
}
BOOL XAudio2SourceVoice_Start(XAudio2SourceVoice* pSourceVoice) {
	SetLastError(0);
	if (!pSourceVoice)
		return FALSE;
	return pSourceVoice->Start();
}
BOOL XAudio2SourceVoice_Stop(XAudio2SourceVoice* pSourceVoice, UINT32 flag) {//0 or XAUDIO2_PLAY_TAILS
	SetLastError(0);
	if (!pSourceVoice)
		return FALSE;
	return pSourceVoice->Stop(flag);
}
BOOL XAudio2SourceVoice_SetVolume(XAudio2SourceVoice* pSourceVoice, FLOAT volume) {
	SetLastError(0);
	if (!pSourceVoice)
		return FALSE;
	return pSourceVoice->SetVolume(volume);
}
VOID XAudio2SourceVoice_GetVolume(XAudio2SourceVoice* pSourceVoice, FLOAT* volume) {
	SetLastError(0);
	if (!pSourceVoice)
		return;
	pSourceVoice->GetVolume(volume);
}
BOOL XAudio2SourceVoice_SetChannelVolumes(XAudio2SourceVoice* pSourceVoice, UINT32 channels, const FLOAT* pVolume) {
	SetLastError(0);
	if (!pSourceVoice)
		return FALSE;
	return pSourceVoice->SetChannelVolumes(channels, pVolume);
}
VOID XAudio2SourceVoice_GetChannelVolumes(XAudio2SourceVoice* pSourceVoice, UINT32 channels, FLOAT* pVolume) {
	SetLastError(0);
	if (!pSourceVoice)
		return;
	pSourceVoice->GetChannelVolumes(channels, pVolume);
}
BOOL XAudio2SourceVoice_QueueFrame(XAudio2SourceVoice* pSourceVoice, const AVFrame* pFrame, BOOL isEof) {
	SetLastError(0);
	if (!pSourceVoice)
		return FALSE;

	return pSourceVoice->QueueFrame(pFrame, isEof);
}
BOOL XAudio2SourceVoice_FlushSourceBuffers(XAudio2SourceVoice* pSourceVoice) {
	SetLastError(0);
	if (!pSourceVoice)
		return FALSE;
	return pSourceVoice->FlushSourceBuffers();
}



#if _DEBUG
DebugAudioSource* DebugAudioSource_Alloc(LPSTR filePath) {
	SetLastError(0);
	DebugAudioSource* pAudioSource = new DebugAudioSource();
	if (pAudioSource->Init(filePath))
	{
		return pAudioSource;
	}
	else
	{
		delete pAudioSource;
		return nullptr;
	}
}
VOID DebugAudioSource_Free(DebugAudioSource** ppAudioSource) {
	if (ppAudioSource)
	{
		DebugAudioSource* pAudioSource = *ppAudioSource;
		if (pAudioSource)
		{
			delete pAudioSource;
			*ppAudioSource = nullptr;
		}
	}
}
BOOL DebugAudioSource_ReadFrame(DebugAudioSource* pAudioSource, AVFrame* pframe) {
	SetLastError(0);
	if (!pAudioSource)
		return FALSE;
	return pAudioSource->ReadFrame(pframe);
}
#endif