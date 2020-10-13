#pragma once
#include "EnemyObject.h"
#define FLOATER_MOVE_SPEED 0.03f;
#define FLOATER_GRAVITY 0.002f;

#define FLOATER_BBOX_WIDTH 18
#define FLOATER_BBOX_HEIGHT 15

#define FLOATER_STATE_MOVE 100

#define FLOATER_STATE_DIE 200

#define FLOATER_STATE_IDLE 300


#define FLOATER_ANI_MOVE_LEFT 0
#define FLOATER_ANI_MOVE_RIGHT 1
class CFloater :public CEnemyObject
{
	
public:
	CFloater();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};
