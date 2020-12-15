#pragma once
#ifndef __SOUND__
#define __SOUND__
#include "Game.h"
#include "dxaudio.h"
#include <map>
using namespace std;

#define SOUND_ID_BULLET_FIRE	1
#define SOUND_ID_BULLET_EXPLOSION	2
class Sound
{
public:
	~Sound();
	static Sound* getInstance();
	void loadSound(HWND hWnd);
	void Play(int soundid);
	void PlayNew(int soundid);					// nếu sound đang phát thì chạy đè lên
	void Stop(int soundid);
	void StopAll();
	void PlayLoop(int soundid);
	bool IsPlaying(int soundid);
	void setVolume(int vol);
	void setMute(bool mute);
	bool getMute();
private:
	Sound();
	static Sound* _instance;
	map<int, CSound*> _listSound;
	float _volume;
	bool _mute;
};

#endif 