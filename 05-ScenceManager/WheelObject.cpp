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
	if (is_Right_Wheel) //right wheel - bánh xe bên phải
	{
		x += 17;
		y -= (MAIN_CHARACTER_BBOX_HEIGHT - 6);
		
		
	}
		
	else if (is_Middle_Wheel) //middle wheel - bánh nằm ở giữa trái và phải
	{
		x += 9;
		y -= 7;
		if (vx != 0) //Add effect if running
		{
			StartUpEffect();
			if (GetTickCount() - up_effect_start > WHEEL_EFFECT_TIME)
			{
				y += 1;
			}	
		}
		if (state == MAIN_CHARACTER_STATE_UP_BARREL) // Nhân vật đưa nòng súng lên
		{
			y+=1;
		}
	}
	else // left wheel - bánh xe bên trái
	{
		y -= (MAIN_CHARACTER_BBOX_HEIGHT-6);
		
	}
		

	
}

void CWheelObject::Render()
{
	int ani = -1;
	int flip = false;
	if (nx > 0)
	{
		flip = false;
	}
	else
		flip = true;
	if (is_Middle_Wheel) // Bánh xe ở giữa
	{
		ani = WHEEL_ANI_IDLE; //Animation chỉ có 1 frame duy nhất
		animation_set->at(ani)->Render(x, y);
	}
	else if (is_Right_Wheel) // Bánh xe bên phải
	{
		ani = WHEEL_ANI_RIGHT_WHEEL; //Animation chỉ có 1 frame duy nhất
		if (vx == 0) // Nhân vật đứng yên
		{
			animation_set->at(ani)->isPause = true; //Dừng animation 
			animation_set->at(ani)->Render(x, y, flip); // Vẽ frame đang bị tạm dừng
		}
		else // Nhân vật di chuyển
		{
			animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
			animation_set->at(ani)->Render(x, y, flip);
		}
	}
	else
	{
			ani = WHEEL_ANI_IDLE_RUN;
			if (vx==0) // Nhân vật đứng yên
			{
				animation_set->at(ani)->isPause = true; //Dừng animation 
				animation_set->at(ani)->Render(x, y, flip); // Vẽ frame đang bị tạm dừng
			}
			else // Nhân vật di chuyển
			{
				animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
				animation_set->at(ani)->Render(x, y, flip);
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