#pragma once
#include "./GameObject.h"
#define WHEEL_WALKING_SPEED		0.15f 
//0.1f
#define WHEEL_JUMP_SPEED_Y		0.5f
#define WHEEL_JUMP_DEFLECT_SPEED 0.2f
#define WHEEL_GRAVITY			0.002f
#define WHEEL_DIE_DEFLECT_SPEED	 0.5f

#define WHEEL_STATE_IDLE			0
#define WHEEL_STATE_WALKING_RIGHT	100
#define WHEEL_STATE_WALKING_LEFT	200
#define WHEEL_STATE_JUMP			300
#define WHEEL_STATE_DIE				400

#define WHEEL_ANI_BIG_IDLE_RIGHT		0
#define WHEEL_ANI_BIG_IDLE_LEFT			1
#define WHEEL_ANI_SMALL_IDLE_RIGHT		2
#define WHEEL_ANI_SMALL_IDLE_LEFT			3

#define WHEEL_ANI_BIG_WALKING_RIGHT			4
#define WHEEL_ANI_BIG_WALKING_LEFT			5
#define WHEEL_ANI_SMALL_WALKING_RIGHT		6
#define WHEEL_ANI_SMALL_WALKING_LEFT		7

#define WHEEL_ANI_DIE				8

#define	WHEEL_LEVEL_SMALL	1
#define	WHEEL_LEVEL_BIG		2

#define WHEEL_BIG_BBOX_WIDTH  15
#define WHEEL_BIG_BBOX_HEIGHT 27

#define WHEEL_SMALL_BBOX_WIDTH  13
#define WHEEL_SMALL_BBOX_HEIGHT 15

#define WHEEL_UNTOUCHABLE_TIME 5000

class CWheelObject : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of WHEEL at scene
	float start_y;

	bool is_Right_Wheel;
	bool is_Middle_Wheel;
	
public:
	CWheelObject(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetIsRightWheel();
	void SetIsMiddleWheel();
};