#pragma once
#include "./GameObject.h"
#include "./MainCharacter.h"

class CBarrelObject : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of BARREL at scene
	float start_y;
	
public:
	CBarrelObject(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	// void SetDelta(D3DXVECTOR2 delta);
};
