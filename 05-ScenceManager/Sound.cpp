

#include "Sound.h"

Sound* Sound::_instance;

Sound* Sound::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Sound();
	}
	return _instance;
}

void Sound::loadSound(HWND hWnd)
{
	// Khởi tạo CSoundManager.
	DirectSound_Init(hWnd);

	CSound* sound = nullptr;

	// DYING
	sound = LoadSound("Sound//bullet_fire.wav");
	_listSound[SOUND_ID_BULLET_FIRE] = sound;

	sound = LoadSound("Sound//bullet_explosion.wav");
	_listSound[SOUND_ID_BULLET_EXPLOSION] = sound;

	//sound = LoadSound("Resources//Sound//Abu Waving.wav");
	//_listSound[int::S_ABU_WAVING] = sound;

	////Thay cho phần ném dao
	//sound = LoadSound("Resources//Sound//Body Crunch.wav");
	//_listSound[int::S_BODY_CRUNCH] = sound;

	//// Menu
	//sound = LoadSound("Resources//Sound//Menu Change.wav");
	//_listSound[int::S_MENU_CHANGE] = sound;

	//sound = LoadSound("Resources//Sound//Menu Select.wav");
	//_listSound[int::S_MENU_SELECT] = sound;

	//sound = LoadSound("Resources//Sound//Menu.wav");
	//_listSound[int::S_MENU] = sound;

	//// Level comple
	//sound = LoadSound("Resources//Sound//LevelComplete.wav");
	//_listSound[int::S_LEVELCOMPLETE] = sound;


	////item
	//sound = LoadSound("Resources//Sound//Apple Collect.wav");
	//_listSound[int::S_APPLE_COLLECT] = sound;

	//sound = LoadSound("Resources//Sound//Clay Pot.wav");
	//_listSound[int::S_CLAY_POT] = sound;

	//sound = LoadSound("Resources//Sound//Wow!.wav");
	//_listSound[int::S_WOW] = sound;

	//sound = LoadSound("Resources//Sound//Guard Beckon.wav");
	//_listSound[int::S_GUARD_BECKON] = sound;

	//sound = LoadSound("Resources//Sound//Gem Collect.wav");
	//_listSound[int::S_GEM_COLLECT] = sound;

	//sound = LoadSound("Resources//Sound//Extra Health.wav");
	//_listSound[int::S_EXTRA_HEALTH] = sound;

	////aladin
	//sound = LoadSound("Resources//Sound//Oooh.wav");
	//_listSound[int::S_OOOH] = sound;

	//sound = LoadSound("Resources//Sound//Low Sword.wav");
	//_listSound[int::S_LOW_SWORD] = sound;

	//sound = LoadSound("Resources//Sound//High Sword.wav");
	//_listSound[int::S_HIGH_SWORD] = sound;

	//sound = LoadSound("Resources//Sound//Object Throw.wav");
	//_listSound[int::S_OBJECT_THROW] = sound;


	//sound = LoadSound("Resources//Sound//Aladdin Hurt.wav");
	//_listSound[int::S_ALADDIN_HURT] = sound;

	//sound = LoadSound("Resources//Sound//Outta Apples.wav");
	//_listSound[int::S_OUTTA_APPLES] = sound;

	////enemy
	//sound = LoadSound("Resources//Sound//Cloud Poof.wav");
	//_listSound[int::S_CLOUD_POOF] = sound;

	//sound = LoadSound("Resources//Sound//Aaah.wav");
	//_listSound[int::S_AAAH] = sound;


	//sound = LoadSound("Resources//Sound//Start Gun.wav");
	//_listSound[int::S_START_GUN] = sound;

	////map object
	//sound = LoadSound("Resources//Sound//Camel Spit.wav");
	//_listSound[int::S_CAMEL_SPIT] = sound;

	//sound = LoadSound("Resources//Sound//Apple Splat.wav");
	//_listSound[int::S_APPLE_SPLAT] = sound;

	//sound = LoadSound("Resources//Sound//Sword Ching.wav");
	//_listSound[int::S_SWORD_CHING] = sound;

	//sound = LoadSound("Resources//Sound//Sword Spinning.wav");
	//_listSound[int::S_SWORD_SPINNING] = sound;

	//sound = LoadSound("Resources//Sound//Wall Hit 2.wav");
	//_listSound[int::S_WALL_HIT_2] = sound;

	//sound = LoadSound("Resources//Sound//Spring Doing 1.wav");
	//_listSound[int::S_SPRING_DOING_1] = sound;

	//sound = LoadSound("Resources//Sound//Spring Doing 2.wav");
	//_listSound[int::S_SPRING_DOING_2] = sound;

	//sound = LoadSound("Resources//Sound//Continue Point.wav");
	//_listSound[int::S_CONTINUE_POINT] = sound;

	//sound = LoadSound("Resources//Sound//Aladdin Push.wav");
	//_listSound[int::S_ALADDIN_PUSH] = sound;

	//sound = LoadSound("Resources//Sound//PrinceAli.wav");
	//_listSound[int::S_PRINCEALI] = sound;

	//sound = LoadSound("Resources//Sound//Fire From Coal.wav");
	//_listSound[int::S_FIRE_FROM_COAL] = sound;

	//// peddler
	//sound = LoadSound("Resources//Sound//Flute Pickup.wav");
	//_listSound[int::S_FLUTE_PICKUP] = sound;

	//sound = LoadSound("Resources//Sound//Honk.wav");
	//_listSound[int::S_HONK] = sound;


	////Boss
	//sound = LoadSound("Resources//Sound//Bones Tinkle.wav");
	//_listSound[int::S_BONES_TINKLE] = sound;

}

bool Sound::IsPlaying(int soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr)
	{
		return sound->IsSoundPlaying();
	}
	return false;
}

void Sound::setVolume(int vol)
{
	_volume = vol;
}

void Sound::setMute(bool mute)
{
	_mute = mute;
}

bool Sound::getMute()
{
	return _mute;
}

void Sound::Play(int soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr && _mute == false)
	{
		PlaySound(sound, _volume);
	}
}
void Sound::PlayNew(int soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr && _mute == false)
	{
		auto newSound = new CSound(*sound);
		PlaySound(newSound);
	}
}
void Sound::PlayLoop(int soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr)
	{
		sound->Play(0, DSBPLAY_LOOPING);
	}
}
void Sound::Stop(int soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr && _mute == false)
	{
		StopSound(sound);
	}
}

void Sound::StopAll()
{
	for (auto sound : _listSound)
	{
		if (sound.second->IsSoundPlaying())
		{
			this->Stop(sound.first);
		}
	}
}

Sound::Sound()
{
	_mute = false;
	_volume = 3;
}

Sound::~Sound()
{
}