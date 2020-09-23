#pragma once
#include "GameObject.h"
class CEnemyObject : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)=0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render()=0;

public:
	CEnemyObject();
	
	virtual void SetState(int state) {};

};