#include "pch.h"
#include "Exports.h"



XAudio2Player* XAudio2Player_Alloc()
{
	XAudio2Player* player = new XAudio2Player();
	if (player->Init())
		return player;
	else
	{
		delete player;
		return nullptr;
	}
}
void XAudio2Player_Free(XAudio2Player** ppxAudio2Player)
{
	if (ppxAudio2Player)
	{
		XAudio2Player* pointer = *ppxAudio2Player;
		if (pointer)
		{
			delete pointer;
			*ppxAudio2Player = nullptr;
		}
	}
}

XAudio2Voice* XAudio2Voice_Alloc(XAudio2Player* pxAudio2Player, const AVFrame* pFrame) {
	if (pxAudio2Player)
	{
		XAudio2Voice* pSource = new XAudio2Voice(pxAudio2Player->GetInterface());
		if (pSource->Init(pFrame))
			return pSource;
		else
		{
			delete pSource;
			return nullptr;
		}
	}
	return nullptr;
}
void XAudio2Voice_Free(XAudio2Voice** ppxAudio2Player) {
	if (ppxAudio2Player)
	{
		XAudio2Voice* pointer = *ppxAudio2Player;
		if (pointer)
		{
			delete pointer;
			*ppxAudio2Player = nullptr;
		}
	}
}
BOOL XAudio2Voice_QueueFrame(XAudio2Voice* pXAudio2Voice, const AVFrame* pFrame) {
	if (pXAudio2Voice && pFrame)
	{
		return pXAudio2Voice->QueueFrame(pFrame);
	}
	return FALSE;
}

#if _DEBUG
void Test() {
	DebugAudioSource* dbgAudio = new DebugAudioSource();
	if (dbgAudio->Init((LPSTR)"D:\\01 Rainbow.mp3"))//D:\\Otaku\\Anime\\Hana to Alice- Satsujin Jiken Full.mp4
	{
		XAudio2Player* player = new XAudio2Player();
		if (!player->Init()) {
			delete player;
			delete dbgAudio;
			return;
		}
		XAudio2Voice* voice = new XAudio2Voice(player->GetInterface());

		AVFrame* pframe = av_frame_alloc();
		while (dbgAudio->ReadFrame(pframe))
		{
			bool r = voice->Init(pframe);

			if (r)
				r = voice->QueueFrame(pframe);

			if (!r)
				break;
		}
		av_frame_free(&pframe);
		voice->QueueFrame(nullptr, true);
		delete voice;
		delete player;
	}
	delete dbgAudio;
}
#endif