#pragma once
#include "./GameObject.h"
#include "./MainCharacter.h"

#define WHEEL_ANI_IDLE		0
#define WHEEL_ANI_RIGHT_WHEEL		2
#define WHEEL_ANI_IDLE_RUN			1
#define WHEEL_EFFECT_TIME 1000
class CWheelObject : public CGameObject
{
	int level;
	int untouchable;
	DWORD up_effect_start;

	float start_x;			// initial position of WHEEL at scene
	float start_y;

	bool is_Right_Wheel;
	bool is_Middle_Wheel;

	bool beingUp;
public:
	CWheelObject(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUpEffect() { y -= 1; up_effect_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetIsRightWheel();
	void SetIsMiddleWheel();
};