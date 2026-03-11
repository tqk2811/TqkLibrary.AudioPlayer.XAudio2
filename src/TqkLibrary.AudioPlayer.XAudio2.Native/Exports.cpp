#include "pch.h"
#include "Exports.h"

int XAudio2_GetAudioDeviceCount() {
	SetLastError(0);
	ComPtr<IMMDeviceEnumerator> pEnumerator;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)pEnumerator.GetAddressOf());
	if (FAILED(hr)) return 0;

	ComPtr<IMMDeviceCollection> pCollection;
	hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, pCollection.GetAddressOf());
	if (FAILED(hr)) return 0;

	UINT count = 0;
	pCollection->GetCount(&count);
	return count;
}

BOOL XAudio2_GetAudioDeviceInfo(int index, AudioDeviceInfo* info) {
	SetLastError(0);
	if (!info) return FALSE;

	ComPtr<IMMDeviceEnumerator> pEnumerator;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)pEnumerator.GetAddressOf());
	if (FAILED(hr)) return FALSE;

	ComPtr<IMMDeviceCollection> pCollection;
	hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, pCollection.GetAddressOf());
	if (FAILED(hr)) return FALSE;

	ComPtr<IMMDevice> pDevice;
	hr = pCollection->Item(index, pDevice.GetAddressOf());
	if (FAILED(hr)) return FALSE;

	LPWSTR pwszID = NULL;
	hr = pDevice->GetId(&pwszID);
	if (FAILED(hr)) return FALSE;

	wcscpy_s(info->szDeviceId, pwszID);
	CoTaskMemFree(pwszID);

	ComPtr<IPropertyStore> pProps;
	hr = pDevice->OpenPropertyStore(STGM_READ, pProps.GetAddressOf());
	if (SUCCEEDED(hr)) {
		PROPVARIANT varName;
		PropVariantInit(&varName);
		hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
		if (SUCCEEDED(hr)) {
			wcscpy_s(info->szDeviceName, varName.pwszVal);
			PropVariantClear(&varName);
		} else {
			info->szDeviceName[0] = L'\0';
		}
	} else {
		info->szDeviceName[0] = L'\0';
	}

	return TRUE;
}

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


XAudio2MasterVoice* XAudio2MasterVoice_Alloc(const XAudio2Engine* pEngine, LPCWSTR szDeviceId, int nb_channels, int sample_rate) {
	SetLastError(0);
	if (!pEngine)
		return nullptr;

	XAudio2MasterVoice* pMasterVocie = new XAudio2MasterVoice(pEngine);
	if (pMasterVocie->Init(szDeviceId, nb_channels, sample_rate))
	{
		return pMasterVocie;
	}
	else
	{
		delete pMasterVocie;
		return nullptr;
	}
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


XAudio2SourceVoice* XAudio2SourceVoice_Alloc(const XAudio2MasterVoice* pMasterVoice, int channels, int sampleRate, int bitsPerSample, BOOL isFloat) {
	SetLastError(0);
	if (!pMasterVoice)
		return nullptr;

	XAudio2SourceVoice* pSourceVoice = new XAudio2SourceVoice(pMasterVoice);
	if (pSourceVoice->Init(channels, sampleRate, bitsPerSample, isFloat))
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
VOID XAudio2SourceVoice_GetVoiceDetails(XAudio2SourceVoice* pSourceVoice, XAUDIO2_VOICE_DETAILS* pVoiceDetails) {
	SetLastError(0);
	if (!pSourceVoice)
		return;
	pSourceVoice->GetVoiceDetails(pVoiceDetails);
}
VOID XAudio2SourceVoice_GetState(XAudio2SourceVoice* pSourceVoice, XAUDIO2_VOICE_STATE* pState, UINT32 flag) {
	SetLastError(0);
	if (!pSourceVoice)
		return;
	pSourceVoice->GetState(pState, flag);
}
XAudio2SourceQueueResult XAudio2SourceVoice_QueueFrame(XAudio2SourceVoice* pSourceVoice, const BYTE* pData, UINT32 dataLength, BOOL isEof) {
	SetLastError(0);
	if (!pSourceVoice)
		return XAudio2SourceQueueResult::XAudio2SourceQueue_Failed;

	return pSourceVoice->QueueFrame(pData, dataLength, isEof);
}
BOOL XAudio2SourceVoice_FlushSourceBuffers(XAudio2SourceVoice* pSourceVoice) {
	SetLastError(0);
	if (!pSourceVoice)
		return FALSE;
	return pSourceVoice->FlushSourceBuffers();
}