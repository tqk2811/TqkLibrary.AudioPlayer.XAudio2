#ifndef _XAudio2VoiceCallback_H_
#define _XAudio2VoiceCallback_H_
class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	XAudio2VoiceCallback();
	~XAudio2VoiceCallback();

	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired);
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd();
	void STDMETHODCALLTYPE OnStreamEnd();
	void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext);
	void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext);
	void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext);
	void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error);

	BOOL WaitReadEnd(DWORD timeout);
	void Reset();

private:
	HANDLE _waitHandle{ NULL };
};
#endif