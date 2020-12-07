
#include "Coupling.h"
#include "Weapon.h"

CCoupling::CCoupling(float start_x, float start_y,bool is_left) :CEnemyObject()
{
	SetState(COUPLING_STATE_MOVE);
	this->is_left = is_left;
	this->start_x = start_x;
	this->start_y = start_y;
	isEnable = true;
	isDisplay = true;
	x = this->start_x;
	y = this->start_y;
}

void CCoupling::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - COUPLING_BBOX_HEIGHT;
			right = x + COUPLING_BBOX_WIDTH;
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

void CCoupling::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	time_moving += dt;
	if (x <= 0 || x >= CGame::GetInstance()->GetScreenWidth() - COUPLING_BBOX_WIDTH - 1)
		SetState(COUPLING_STATE_MOVE_CHANGE_DIRECTION_X);
	CGameObject::Update(dt);
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(COUPLING_STATE_DIE);
	}
	x += dx;
	y += dy;

}

void CCoupling::Render()
{
	if (isEnable)
	{

		int ani=-1;
		if(is_left)
			ani= COUPLING_ANI_LEFT;
		else
			ani = COUPLING_ANI_RIGHT;
		if (isDisplay)
		{
			animation_set->at(ani)->Render(x, y);
			//RenderBoundingBox();
		}
	}
}

void CCoupling::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COUPLING_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case COUPLING_STATE_MOVE:
		vx = COUPLING_MOVE_SPEED;
		break;
	case COUPLING_STATE_MOVE_CHANGE_DIRECTION_X:
		vx = -vx;
		break;
	case COUPLING_STATE_MOVE_CHANGE_DIRECTION_Y:
		vy = -vy;
		break;
	case COUPLING_STATE_ATTACK:
		break;
	case COUPLING_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;
	case STATE_ITEM:
		vx = 0;
		vy = 0;
		isDisplay = false;
		break;
	default:
		break;
	}
}