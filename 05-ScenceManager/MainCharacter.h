#pragma once
#include "GameObject.h"
#include "BarrelObject.h"
#include "WheelObject.h"
#include "CabinObject.h"
#include "Weapon.h"
#include "Human.h"
#define MAIN_CHARACTER_RUN_SPEED		0.15f 
//0.1f
#define MAIN_CHARACTER_JUMP_SPEED_Y		0.5f
#define HUMAN_SMALL_JUMP_SPEED_Y		0.4f
#define MAIN_CHARACTER_JUMP_DEFLECT_SPEED 0.2f
#define MAIN_CHARACTER_GRAVITY			0.002f
#define MAIN_CHARACTER_DIE_DEFLECT_SPEED	 0.5f

#define MAIN_CHARACTER_STATE_IDLE			0
#define MAIN_CHARACTER_STATE_RUN_RIGHT	100


#define MAIN_CHARACTER_STATE_RUN_LEFT	200
#define MAIN_CHARACTER_STATE_JUMP			300
#define MAIN_CHARACTER_STATE_DIE				400
#define MAIN_CHARACTER_STATE_UP_BARREL				500
#define MAIN_CHARACTER_STATE_DOWN_BARREL				501
#define MAIN_CHARACTER_STATE_NONE_COLLISION			600
#define MAIN_CHARACTER_STATE_HUMAN					800
//#define MAIN_CHARACTER_STATE_DOWN_BARREL				600


#define MAIN_CHARACTER_STATE_BARREL_FIRE				700
#define MAIN_CHARACTER_STATE_BARREL_FIRE_UP				701

#define MAIN_CHARACTER_ANI_IDLE_RIGHT		0
#define MAIN_CHARACTER_ANI_IDLE_LEFT			0


#define MAIN_CHARACTER_ANI_RUN_RIGHT			1
#define MAIN_CHARACTER_ANI_RUN_LEFT			1
#define MAIN_CHARACTER_ANI_JUMP			1

#define MAIN_CHARACTER_ANI_DIE				8

#define	MAIN_CHARACTER_LEVEL_SMALL	1
#define	MAIN_CHARACTER_LEVEL_BIG		2


#define MAIN_CHARACTER_BBOX_WIDTH  24
#define MAIN_CHARACTER_BBOX_HEIGHT 16

class CMainCharacter: public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of MAIN_CHARACTER at scene
	float start_y;

	//Bo sung them object
	vector<LPGAMEOBJECT> componentObjects;
	vector<CWeapon*> list_weapon;

	int blood;
	int alive;
	int heart;
	int score;
	
public:

	//Bo sung property
	bool Is_On_Ground;
	bool Is_Human;
	CMainCharacter(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	
	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void AddComponentObject(CGameObject* object);
	vector<LPGAMEOBJECT> GetComponentObjects();
	void ClearObjects() { componentObjects.clear(); list_weapon.clear(); }
	
	float GetStartX() { return start_x; }
	float GetStartY() { return start_y; }
};