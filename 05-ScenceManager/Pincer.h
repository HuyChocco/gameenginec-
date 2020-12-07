#pragma once
#include "EnemyObject.h"
#include "Game.h"
#define PINCER_MOVE_SPEED 0.05f;
#define PINCER_GRAVITY 0.002f;

#define PINCER_BBOX_WIDTH 18
#define PINCER_BBOX_HEIGHT 32

#define PINCER_STATE_MOVE 100
#define PINCER_STATE_MOVE_CHANGE_DIRECTION_X 101
#define PINCER_STATE_MOVE_CHANGE_DIRECTION_Y 102
#define PINCER_STATE_ATTACK 103

#define PINCER_STATE_DIE 200

#define PINCER_STATE_IDLE 300
#define PINCER_ANI_RIGHT 3
#define PINCER_ANI_LEFT 5

#define TIME_CHANGE_DIRECTION 1000
class CPincer :public CEnemyObject
{
	DWORD time_moving;
	bool isBeingDown;
	bool isBeingUp;
	bool isRepeat;
	float start_x;
	float start_y;
	bool is_left;
public:
	CPincer(float start_x, float start_y,bool is_left);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

};
