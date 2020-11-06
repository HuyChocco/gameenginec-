#include "Teleporter.h"

CTeleporter::CTeleporter(int _item) :CEnemyObject()
{
	SetState(TELEPORTER_STATE_IDLE);
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
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(TELEPORTER_STATE_DIE);
	}
	//if (isEnable)
	{
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
		if (!isFireVertical)
		{
			ani = TELEPORTER_STATE_UNCLOAK;
			animation_set->at(ani)->Render(x, y);

			if (animation_set->at(ani)->isFinish)
			{
				isFireVertical = !isFireVertical;
				animation_set->at(ani)->isFinish = false;
				SetState(TELEPORTER_STATE_UNCLOAK);
			}

		}
		else
		{
			ani = TELEPORTER_STATE_ATTACK;
			animation_set->at(ani)->Render(x, y);

			if (animation_set->at(ani)->isFinish)
			{
				isFireVertical = !isFireVertical;
				animation_set->at(ani)->isFinish = false;
				SetState(TELEPORTER_STATE_ATTACK);
			}
		}
		RenderBoundingBox();

	}
	for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Render();
	}
}

void CTeleporter::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TELEPORTER_STATE_IDLE:
		vx = 0;
		break;
	case TELEPORTER_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;
	default:
		break;
	}
}

