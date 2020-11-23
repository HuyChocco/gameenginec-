#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"

#include "Brick.h"
#include "Stair.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "EnemyObject1.h"
#include "Worm.h"
#include "Spider.h"
#include "Cannon.h"
#include "Spike.h"
#include "Eyeball.h"
#include "Teleporter.h"
#include "Floater.h"
#include "Dome.h"
#include "Jumper.h"
#include "Orb.h"
//bo sung
#include "MainCharacter.h"
#include "BarrelObject.h"
#include "WheelObject.h"
#include "CabinObject.h"
#include "Human.h"
#include "Vehicle.h"

#include "TiledMap.h"
#include "Grid.h"


class CPlayScene: public CScene
{
protected: 
	//CMario *player;	
	CMainCharacter* player;// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;

	vector<LPGAMEOBJECT> hub_objects;
	
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	//Bổ sung map
	void _ParseSection_MAP(string line);
	void _ParseSection_GRID(string line);

	CMap* tiledMap;
	int id_next_map=-1;
	int id_pre_map = -1;
	LPCWSTR tiled_map_file_path_next;
	CSprites* sprites_next_map;



public: 
	bool isRenderNextMap = false;
	bool isRenderPreMap = false;
	bool initNextMap = true;
	bool initGridFlag = true;
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//CMario * GetPlayer() { return player; } 
	CMainCharacter* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;

	virtual CMap* GetMap();
	virtual void GetNextMap();

	
	CSprites* GetSpritesNextMap() { return sprites_next_map; }


};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

