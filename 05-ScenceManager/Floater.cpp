<<<<<<< HEAD
﻿#include "Floater.h"
=======
﻿
#include "Floater.h"
#include "Weapon.h"
>>>>>>> master

CFloater::CFloater(int _item) :CEnemyObject()
{
	SetState(FLOATER_STATE_MOVE);// vx > 0
	this->blood = 1;
	item = _item;
	time_moving = 0;
	isEnable = true;
	isDisplay = true;
	
}

void CFloater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - FLOATER_BBOX_HEIGHT;
			right = x + FLOATER_BBOX_WIDTH;
		}
		else
		{
			if (item == 1)
			{
				top = y - ITEM_P_BBOX_HEIGHT;

				right = x + ITEM_P_BBOX_WIDTH;
			}

		}
		bottom = y;
	}
	
}

void CFloater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	

	time_moving += dt;
	if (isRepeat) // nếu isRepeat = true thì vào 
	{
		if (time_moving >= TIME_CHANGE_DIRECTION) // nếu thời gian vẽ frame > thời gian đi trục x số frame = thời gian trục x /time_moving : vẽ trục x xong
		{
			SetState(FLOATER_STATE_MOVE_CHANGE_DIRECTION_Y); // cho chạy trục y vy =- vy : chạy ngược lại : chạy xuống lúc đầu
			isBeingDown = false; // đang xuống  = false
			time_moving = 0; // chạy xong bắt đầu đếm lại
			isRepeat = false; // lặp lại = false
		}
	}
	else // nếu isRepeat = false
	{
		if (!isBeingDown) // nếu đang xuống = false
		{
			if (time_moving >= TIME_CHANGE_DIRECTION)// nếu thời gian vẽ frame > thời gian vẽ trục y : vẽ trục y xong
			{
				SetState(FLOATER_STATE_MOVE_CHANGE_DIRECTION_X);// cho chạy trục x vx = -vx
				isBeingDown = true;// đi xuống = true
				time_moving = 0;// lặp lại
			}
		}
		else // nếu đang xuống = true
		{
			if (isBeingUp) // đang đi lên = true
			{
				if (time_moving >= TIME_CHANGE_DIRECTION) // nếu thời gian vẽ > vẽ trục y 
				{
					SetState(FLOATER_STATE_MOVE_CHANGE_DIRECTION_X); // vẽ trục x vx = -vx
					isBeingUp = false;// đang lên = false
					time_moving = 0;// đếm lại
					isRepeat = true;// lặp lại = true
				}
			}
			else// đang đi lên = false
			{
				if (time_moving >= TIME_CHANGE_DIRECTION)// nếu vẽ xong trục x
				{
<<<<<<< HEAD
					SetState(FLOATER_STATE_MOVE_CHANGE_DIRECTION_Y);// cho vẽ trục y vy = -vy
					isBeingUp = true;// đang lên = true
					time_moving = 0;// 
=======
					SetState(FLOATER_STATE_MOVE_CHANGE_DIRECTION_Y);
					isBeingUp = true;
					time_moving = 0;
					SetState(FLOATER_STATE_ATTACK);
>>>>>>> master
				}
			}

		}
	}
	
	// Calculate dx, dy 
	CGameObject::Update(dt);
	
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(FLOATER_STATE_DIE);
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (isDisplay)
		CalcPotentialCollisions(coObjects, coEvents);
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = -vx;
		if (ny != 0) vy = -vy;

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Update(dt, coObjects);
	}
	
}

void CFloater::Render()
{
	if (isEnable)
	{

		int ani = -1;
		switch (state)
		{
		case FLOATER_STATE_ATTACK:
		case FLOATER_STATE_MOVE_CHANGE_DIRECTION_X:
		case FLOATER_STATE_MOVE_CHANGE_DIRECTION_Y:
		case FLOATER_STATE_IDLE:
		case FLOATER_STATE_MOVE:
			if (nx > 0)
				ani = FLOATER_ANI_MOVE_RIGHT;
			else
				ani = FLOATER_ANI_MOVE_LEFT;
			break;
		case STATE_ITEM:
			ani = item;
			animation_item_set->at(ani - 1)->Render(x, y);
			break;
		}
		if (isDisplay)
		{
			animation_set->at(ani)->Render(x, y);
			RenderBoundingBox();
		}
		
	}
	for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Render();
	}

}

void CFloater::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLOATER_STATE_IDLE:
		vx = 0;
		break;
	case FLOATER_STATE_MOVE:
		if (nx > 0)
		{
			vx = FLOATER_MOVE_SPEED;
		}
		else
		{
			vx = -FLOATER_MOVE_SPEED;
		}
		vy = -FLOATER_MOVE_SPEED;
		break;
	case FLOATER_STATE_MOVE_CHANGE_DIRECTION_X:
		vx = -vx;
		break;
	case FLOATER_STATE_MOVE_CHANGE_DIRECTION_Y:
		vy = -vy;
		break;
	case FLOATER_STATE_ATTACK:
		{
			if (isDisplay)
			{
				CWeapon* weapon = new CWeapon(WEAPON_TYPE_ENEMY_FLOATER);// Khởi tạo weapon
				weapon->SetDirection(nx);
				weapon->SetPosition(x+FLOATER_BBOX_WIDTH/2, y);
				weapon->SetState(WEAPON_FLOATER_STATE_FLY);
				if(player)
					weapon->SetPlayerObject(player);
				list_weapon.push_back(weapon);
			}
			
		}
		break;
	case FLOATER_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;
	case STATE_ITEM:
		vx=0;
		vy=0;
		isDisplay = false;
		break;
	default:
		break;
	}
}