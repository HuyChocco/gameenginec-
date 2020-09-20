#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "WheelObject.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CWheelObject::CWheelObject(float x, float y) : CGameObject()
{
	
	SetState(MAIN_CHARACTER_STATE_IDLE);

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
	int ani = -1;
	int flip = false;
	if (is_Middle_Wheel)
	{
		
		
		animation_set->at(0)->Render(x, y);
	}	
	else
	{
		switch (state)
		{
		case MAIN_CHARACTER_STATE_RUN_RIGHT:
			ani = MAIN_CHARACTER_ANI_RUN_RIGHT;
			flip = false;
			break;
		case MAIN_CHARACTER_STATE_RUN_LEFT:
			ani = MAIN_CHARACTER_ANI_RUN_LEFT;
			flip = true;
			break;
		case MAIN_CHARACTER_STATE_IDLE:
		{
			if (nx > 0)
			{
				ani = MAIN_CHARACTER_ANI_IDLE_RIGHT;
				flip = false;
			}
				
			else
			{
				ani = MAIN_CHARACTER_ANI_IDLE_LEFT;
				flip = true;
			}
				
		}
		break;
		case MAIN_CHARACTER_STATE_JUMP:
			ani = MAIN_CHARACTER_ANI_JUMP;
			break;
		case MAIN_CHARACTER_STATE_DIE:
			break;
		}
		animation_set->at(ani)->Render(x, y,flip);
	}
		
}

void CWheelObject::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == MAIN_CHARACTER_STATE_RUN_RIGHT)
		nx = 1;
	else if (state == MAIN_CHARACTER_STATE_RUN_LEFT)
		nx = -1;
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