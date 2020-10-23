#pragma once
#include "GameObject.h"
//#include "Weapon.h"
class CEnemyObject : public CGameObject
{
protected:
	int blood;

public:
	CEnemyObject();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() = 0;
	virtual void SetState(int state) {};
	void LostBlood(int _blood) { this->blood -= _blood; }

	int GetBlood() { return this->blood; }

};