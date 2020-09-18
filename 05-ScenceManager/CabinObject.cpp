#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "CabinObject.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CCabinObject::CCabinObject(float x, float y) : CGameObject()
{
	
	SetState(CABIN_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CCabinObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += 7;
	

}

void CCabinObject::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CCabinObject::SetState(int state)
{
	
}

void CCabinObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	
}

/*
	Reset CABIN status to the beginning state of a scene
*/
void CCabinObject::Reset()
{
	
}

