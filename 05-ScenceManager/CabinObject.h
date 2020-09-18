#pragma once
#include "./GameObject.h"
#define CABIN_WALKING_SPEED		0.15f 
//0.1f
#define CABIN_JUMP_SPEED_Y		0.5f
#define CABIN_JUMP_DEFLECT_SPEED 0.2f
#define CABIN_GRAVITY			0.002f
#define CABIN_DIE_DEFLECT_SPEED	 0.5f

#define CABIN_STATE_IDLE			0
#define CABIN_STATE_WALKING_RIGHT	100
#define CABIN_STATE_WALKING_LEFT	200
#define CABIN_STATE_JUMP			300
#define CABIN_STATE_DIE				400

#define CABIN_ANI_BIG_IDLE_RIGHT		0
#define CABIN_ANI_BIG_IDLE_LEFT			1
#define CABIN_ANI_SMALL_IDLE_RIGHT		2
#define CABIN_ANI_SMALL_IDLE_LEFT			3

#define CABIN_ANI_BIG_WALKING_RIGHT			4
#define CABIN_ANI_BIG_WALKING_LEFT			5
#define CABIN_ANI_SMALL_WALKING_RIGHT		6
#define CABIN_ANI_SMALL_WALKING_LEFT		7

#define CABIN_ANI_DIE				8

#define	CABIN_LEVEL_SMALL	1
#define	CABIN_LEVEL_BIG		2

#define CABIN_BIG_BBOX_WIDTH  15
#define CABIN_BIG_BBOX_HEIGHT 27

#define CABIN_SMALL_BBOX_WIDTH  13
#define CABIN_SMALL_BBOX_HEIGHT 15

#define CABIN_UNTOUCHABLE_TIME 5000

class CCabinObject : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of CABIN at scene
	float start_y;

	
public:
	CCabinObject(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	//void SetDelta(D3DXVECTOR2 delta);
};