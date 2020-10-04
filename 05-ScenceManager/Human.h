#pragma once
#include "./GameObject.h"
#include "./MainCharacter.h"

#define HUMAN_ANI_IDLE		0
#define HUMAN_ANI_UP			2
#define HUMAN_ANI_DOWN			1

#define	HUMAN_LEVEL_SMALL	1
#define	HUMAN_LEVEL_BIG		2

#define HUMAN_BIG_BBOX_WIDTH  15
#define HUMAN_BIG_BBOX_HEIGHT 27

#define HUMAN_SMALL_BBOX_WIDTH  13
#define HUMAN_SMALL_BBOX_HEIGHT 15

class CHuman : public CGameObject
{
	int level;
	
	float start_x;			// initial position of Human at scene
	float start_y;
	
public:
	CHuman(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	
	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


};
