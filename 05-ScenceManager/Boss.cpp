
#include "Boss.h"
#include "Weapon.h"

CBoss::CBoss(int _item) :CEnemyObject()
{
	SetState(BOSS_STATE_MOVE);
	this->blood = 1;
	item = _item;
	time_moving = 0;
	isEnable = true;
	isDisplay = true;

}

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - BOSS_BBOX_HEIGHT;
			right = x + BOSS_BBOX_WIDTH;
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

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{


	time_moving += dt;
	if (x<=0||x >= CGame::GetInstance()->GetScreenWidth() - BOSS_BBOX_WIDTH)
		SetState(BOSS_STATE_MOVE_CHANGE_DIRECTION_X);
	// Calculate dx, dy 
	CGameObject::Update(dt);

	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(BOSS_STATE_DIE);
	}
	
	x += dx;
	y += dy;


	for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Update(dt, coObjects);
	}

}

void CBoss::Render()
{
	if (isEnable)
	{

		int ani = -1;
		switch (state)
		{
		case BOSS_STATE_ATTACK:
		case BOSS_STATE_MOVE_CHANGE_DIRECTION_X:
		case BOSS_STATE_MOVE_CHANGE_DIRECTION_Y:
		case BOSS_STATE_IDLE:
		case BOSS_STATE_MOVE:
			ani = BOSS_ANI_MOVE_LEFT;
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

void CBoss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STATE_IDLE:
		vx = 0;
		break;
	case BOSS_STATE_MOVE:
		vx = BOSS_MOVE_SPEED;
		break;
	case BOSS_STATE_MOVE_CHANGE_DIRECTION_X:
		vx = -vx;
		break;
	case BOSS_STATE_MOVE_CHANGE_DIRECTION_Y:
		vy = -vy;
		break;
	case BOSS_STATE_ATTACK:
	{
		if (isDisplay)
		{
			//CWeapon* weapon = new CWeapon(WEAPON_TYPE_ENEMY_BOSS);// Khởi tạo weapon
			//weapon->SetDirection(nx);
			//weapon->SetPosition(x + BOSS_BBOX_WIDTH / 2, y);
			//weapon->SetState(WEAPON_BOSS_STATE_FLY);
			//if (player)
			//	weapon->SetPlayerObject(player);
			//list_weapon.push_back(weapon);
		}

	}
	break;
	case BOSS_STATE_DIE:
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