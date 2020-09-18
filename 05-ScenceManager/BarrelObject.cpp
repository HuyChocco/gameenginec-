#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "BarrelObject.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CBarrelObject::CBarrelObject(float x, float y) : CGameObject()
{
	
	SetState(BARREL_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CBarrelObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += 2;
}

void CBarrelObject::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CBarrelObject::SetState(int state)
{
	
}

void CBarrelObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	
}

/*
	Reset BARREL status to the beginning state of a scene
*/
void CBarrelObject::Reset()
{
	
}

