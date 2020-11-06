#pragma once
#include "GameObject.h"
#define POWERHUB_ANI_TEXT 0
#define POWERHUB_ANI_LEVEL 1
class CPowerHub :public CGameObject
{
	int power;
	
public:
	CPowerHub();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};