#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "BarrelObject.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CBarrelObject::CBarrelObject(float x, float y) : CGameObject()
{
	
	SetState(MAIN_CHARACTER_STATE_IDLE);
	this->is_finish_animation = true;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CBarrelObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (nx > 0)
	{
		x += 16;
		y += 2;
	}
	else
		y += 2;
}

void CBarrelObject::Render()
{
	bool flip = false;
	int ani = 0;//default
	if (nx > 0)
	{
		flip = true;
	}
	else
		flip = false;

	switch (state)
	{

	case MAIN_CHARACTER_STATE_UP_BARREL:
		ani = BARREL_ANI_UP;
		animation_set->at(ani)->isRepeat = false;
		break;
	case MAIN_CHARACTER_STATE_DOWN_BARREL:
		ani = BARREL_ANI_DOWN;
		animation_set->at(BARREL_ANI_UP)->SetCurrentFrame(-1);
		animation_set->at(BARREL_ANI_UP)->isRepeat = true;
		break;
	default:
		ani = 0;
		animation_set->at(BARREL_ANI_UP)->SetCurrentFrame(-1);
		animation_set->at(BARREL_ANI_UP)->isRepeat = true;
	}
	

	
	
	animation_set->at(ani)->Render(x, y, flip);
	
}

void CBarrelObject::SetState(int state)
{
	CGameObject::SetState(state);
	
	
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

