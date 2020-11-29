#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "TiledMap.h"

#include "Boss.h"
#include "MainCharacter.h"
#include "Human.h"
#include "GunHub.h"
#include "PowerHub.h"
class CBossScence : public CScene
{
protected:
	CMainCharacter* player;// A play scene has to have player, right? 
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> hub_objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	CMap* tiledMap;
public:

	CBossScence(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Load(int _alive, int _power) {};
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMainCharacter* GetPlayer() { return player; }
};

class CBossScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CBossScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

