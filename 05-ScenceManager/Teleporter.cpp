#include "Teleporter.h"
#include "Weapon.h"
#include "Spike.h"

CTeleporter::CTeleporter(int _item) :CEnemyObject()
{
	SetState(TELEPORTER_STATE_IDLE);
	timeWaitingToMove = 0;
	timeMoving = 0;
	item = _item;
	this->blood = 1;
	isEnable = true;
	isDisplay = true;
}

void CTeleporter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - TELEPORTER_BBOX_HEIGHT;
			right = x + TELEPORTER_BBOX_WIDTH;
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

void CTeleporter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable)
	{
		if (this->blood < 0)
		{
			if (item > 0)
				SetState(STATE_ITEM);
			else
				SetState(TELEPORTER_STATE_DIE);
		}
		// Calculate dx, dy 
		CGameObject::Update(dt);

		if (!isMoving)
			timeWaitingToMove += dt;

		if (timeWaitingToMove >= TIME_START_MOVING)
		{
			SetState(TELEPORTER_STATE_MOVE);
			isStartMoving = true;

		}

		if (isStartMoving)
		{
			timeMoving += dt;
			if (timeMoving >= TIME_MOVING)
			{
				SetState(TELEPORTER_STATE_UNCLOAK);
				isStartMoving = false;
				timeWaitingToMove = 0;
				isMoving = false;
				timeMoving = 0;
			}
			x += dx;
			y += dy;
		}
		for (int i = 0; i < list_weapon.size(); i++)
		{
			list_weapon[i]->Update(dt, coObjects);
		}



	}

}

void CTeleporter::Render()
{
	if (isEnable)
	{
		int ani = -1;
		bool flip = false;
		if (nx > 0)
		{
			flip = true;
		}
		else
			flip = false;
		ani = TELEPORTER_ANI_LEFT;
		switch (state)
		{
		case TELEPORTER_STATE_MOVE:
			break;
		case TELEPORTER_STATE_UNCLOAK:
		{
			ani = TELEPORTER_ANI_ATTACK;
			if (animation_set->at(ani)->isFinish)
			{
				animation_set->at(ani)->isFinish = false;
				SetState(TELEPORTER_STATE_ATTACK);
			}
		}
		break;
		case TELEPORTER_STATE_IDLE:
			break;
		}
		animation_set->at(ani)->Render(x, y, flip);
		RenderBoundingBox();
		for (int i = 0; i < list_weapon.size(); i++)
		{
			list_weapon[i]->Render();
		}
	}

}

void CTeleporter::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TELEPORTER_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case TELEPORTER_STATE_MOVE:
		if (nx > 0)
		{
			vx = TELEPORTER_MOVE_SPEED;
		}
		else
		{
			vx = -TELEPORTER_MOVE_SPEED;
		}

		if (ny > 0)
		{
			vy = TELEPORTER_MOVE_SPEED;
		}
		else
		{
			vy = -TELEPORTER_MOVE_SPEED;
		}
		break;
	case TELEPORTER_STATE_DIE:
		isEnable = false;
		isDisplay = false;
		break;
	case TELEPORTER_STATE_CLOAK:
		vx = 0;
		vy = 0;
		break;
	case TELEPORTER_STATE_ATTACK:
	{
		CWeapon* weapon = new CWeapon(WEAPON_TYPE_ENEMY_TELEPORTER);// Khởi tạo weapon
		weapon->SetPosition(x, y);
		weapon->SetDirection(nx);
		weapon->SetState(WEAPON_TELEPORTER_STATE_FLY);
		list_weapon.push_back(weapon);
	}

	break;
	case STATE_ITEM:
		if (item > 0)
		{
			hasItem = true;
		}
		vy = 0;
		vx = 0;
		isDisplay = false;
		break;
	default:
		break;
	}
}