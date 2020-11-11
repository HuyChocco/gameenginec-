#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Vehicle.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CVehicle::CVehicle(float x, float y) : CGameObject() 
{

	vehicle_nx = nx;
	SetState(MAIN_CHARACTER_STATE_IDLE);

	start_x = x;
	start_y = y;

	this->x = x;
	this->y = y;
}

void CVehicle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}

void CVehicle::Render()
{
	
	int alpha = 255;
	if (untouchable)
		alpha = 128;
	bool flip = false;
	int ani = -1;
	if (nx > 0)
	{
		flip = false;
	}
	else
		flip = true;
	ani = VEHICLE_ANI_MOVE;
	switch (state)
	{
	case MAIN_CHARACTER_STATE_STRAIGHT_BARREL:
	case MAIN_CHARACTER_STATE_UP_BARREL:
	{
		ani = VEHICLE_ANI_NONG_SUNG;
		animation_set->at(ani)->isRepeat = false;
		is_barrel_up = true;//flag to determine suitable animation
		animation_set->at(ani)->Render(x, y, flip, alpha);
		if (animation_set->at(ani)->isFinish)
			isBarrelStraight = true;
		else
			isBarrelStraight = false;
		
	}
	break;
	default:
	{
		animation_set->at(VEHICLE_ANI_NONG_SUNG)->isFinish = false;
		ani = VEHICLE_ANI_MOVE;
		animation_set->at(VEHICLE_ANI_NONG_SUNG)->SetCurrentFrame(-1);
		animation_set->at(VEHICLE_ANI_NONG_SUNG)->isRepeat = true;
		is_barrel_up = false;
		isBarrelStraight = false;
		if (vehicle_nx != nx)
		{
			int _ani = VEHICLE_ANI_XOAY;
			animation_set->at(_ani)->Render(x, y, flip, alpha);
			if (animation_set->at(_ani)->isFinish)
			{
				animation_set->at(ani)->Render(x, y, flip, alpha);
				vehicle_nx = nx;
				animation_set->at(_ani)->isFinish = false;
			}
		}
		else
		{
			animation_set->at(ani)->Render(x, y, flip, alpha);

		}
	}
	break;
	}
	
}

void CVehicle::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MAIN_CHARACTER_STATE_STRAIGHT_BARREL:
	case MAIN_CHARACTER_STATE_UP_BARREL:
		break;
	default:
		is_barrel_up = false;
		isBarrelStraight = false;
		break;
	}
}

void CVehicle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{


}

/*
	Reset CABIN status to the beginning state of a scene
*/
void CVehicle::Reset()
{

}

