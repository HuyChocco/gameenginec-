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
	CGameObject::Update(dt);
}

void CWheelObject::Render()
{
	int alpha = 255;
	if (untouchable)
		alpha = 128;
	int ani = -1;
	int flip = false;
	if (nx > 0)
	{
		flip = false;
	}
	else
		flip = true;
	
	if (is_Right_Wheel) // Bánh xe bên phải
	{
		
		ani = WHEEL_ANI_RIGHT_WHEEL; //Animation chỉ có 1 frame duy nhất
		if (vx == 0) // Nhân vật đứng yên
		{
			animation_set->at(ani)->isPause = true; //Dừng animation 
			animation_set->at(ani)->Render(x+MAIN_CHARACTER_BBOX_WIDTH- WHEEL_BBOX_WIDTH, y- MAIN_CHARACTER_BBOX_HEIGHT+ WHEEL_BBOX_HEIGHT, flip,alpha); // Vẽ frame đang bị tạm dừng
		}
		else // Nhân vật di chuyển
		{
			animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
			animation_set->at(ani)->Render(x + MAIN_CHARACTER_BBOX_WIDTH - WHEEL_BBOX_WIDTH, y - MAIN_CHARACTER_BBOX_HEIGHT + WHEEL_BBOX_HEIGHT, flip,alpha);
		}
	}
	else //Bánh xe bên trái
	{
			ani = WHEEL_ANI_IDLE_RUN;
			if (vx==0) // Nhân vật đứng yên
			{
				animation_set->at(ani)->isPause = true; //Dừng animation 
				animation_set->at(ani)->Render(x, y - MAIN_CHARACTER_BBOX_HEIGHT + WHEEL_BBOX_HEIGHT, flip,alpha); // Vẽ frame đang bị tạm dừng
			}
			else // Nhân vật di chuyển
			{
				animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
				animation_set->at(ani)->Render(x, y - MAIN_CHARACTER_BBOX_HEIGHT + WHEEL_BBOX_HEIGHT, flip,alpha);
			}	
	}
		
}

void CWheelObject::SetState(int state)
{
	CGameObject::SetState(state);
	
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
// Thiết lập đây là bánh xe nằm bên phải
void CWheelObject::SetIsRightWheel()
{
	is_Right_Wheel = true; 
}
// Thiết lập đây là bánh xe nằm ở giữa
void CWheelObject::SetIsMiddleWheel()
{
	is_Middle_Wheel = true;
}