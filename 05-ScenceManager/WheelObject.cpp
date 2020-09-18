#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "WheelObject.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CWheelObject::CWheelObject(float x, float y) : CGameObject()
{
	
	SetState(WHEEL_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	is_Right_Wheel = false;
	is_Middle_Wheel = false;
}

void CWheelObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (is_Right_Wheel)
	{
		x += 17;
		y += 11;
	}
		
	else if (is_Middle_Wheel)
	{
		x += 9;
		y += 6;
	}
	else
		y += 11;
}

void CWheelObject::Render()
{
	if(is_Middle_Wheel)
		animation_set->at(0)->Render(x, y);
	else
		animation_set->at(1)->Render(x, y);
}

void CWheelObject::SetState(int state)
{
	
}

void CWheelObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{


}

/*
	Reset WHEEL status to the beginning state of a scene
*/
void CWheelObject::Reset()
{
	
}

void CWheelObject::SetIsRightWheel()
{
	is_Right_Wheel = true;
}

void CWheelObject::SetIsMiddleWheel()
{
	is_Middle_Wheel = true;
}