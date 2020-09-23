#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "CabinObject.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CCabinObject::CCabinObject(float x, float y) : CGameObject()
{
	
	SetState(MAIN_CHARACTER_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CCabinObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (nx < 0)
	{
		x += 7;
	}

}

void CCabinObject::Render()
{
	bool flip = false;
	if (nx > 0)
	{
		flip = true;
	}
	else
		flip = false;
	animation_set->at(0)->Render(x, y,flip);
}

void CCabinObject::SetState(int state)
{
	CGameObject::SetState(state);
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

