
#include "Boss.h"
#include "Weapon.h"
#include "Sound.h"

CBoss::CBoss(float x,float y,int _item) :CEnemyObject()
{
	SetState(BOSS_STATE_IDLE);
	this->blood = 20;
	item = _item;
	time_moving = 0;
	time_moving_coupling = 0;
	isEnable = true;
	isDisplay = true;
	start_x = x;
	start_y = y;
	CreateCouplingElements();
}

void CBoss::CreateCouplingElements()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set;
	for (int i = 0; i < 4; i++)
	{
		CCoupling* object_right = new CCoupling(start_x+BOSS_BBOX_WIDTH,start_y-BOSS_BBOX_HEIGHT/2-i*COUPLING_BBOX_HEIGHT,false);
		ani_set = animation_sets->Get(3);
		object_right->SetAnimationSet(ani_set);
		object_right->SetIndex(i);
		right_coupling_elements.push_back(object_right);
	}
	for (int i = 0; i < 4; i++)
	{
		CCoupling* object_left = new CCoupling(start_x, start_y - BOSS_BBOX_HEIGHT / 2 - i * COUPLING_BBOX_HEIGHT,true);
		ani_set = animation_sets->Get(3);
		object_left->SetAnimationSet(ani_set);
		object_left->SetIndex(i);
		left_coupling_elements.push_back(object_left);
	}
	CPincer* object_right = new CPincer(start_x+BOSS_BBOX_WIDTH, start_y - BOSS_BBOX_HEIGHT / 2 - 4 * COUPLING_BBOX_HEIGHT,false);
	ani_set = animation_sets->Get(3);
	object_right->SetAnimationSet(ani_set);
	right_pincer = object_right;

	CPincer* object_left = new CPincer(start_x, start_y - BOSS_BBOX_HEIGHT / 2 - 4 * COUPLING_BBOX_HEIGHT,true);
	ani_set = animation_sets->Get(3);
	object_left->SetAnimationSet(ani_set);
	left_pincer = object_left;
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
	if (state != BOSS_STATE_IDLE)
	{
		time_moving += dt;
		if (time_moving >= 2000)
		{
			time_moving = 0;
			SetState(BOSS_STATE_ATTACK);
		}
	}
	if (x<=0||x >= CGame::GetInstance()->GetScreenWidth() - BOSS_BBOX_WIDTH-1)
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
	if (state != BOSS_STATE_IDLE)
	{
		time_moving_coupling += dt;
		time_moving_coupling_left += dt;
		for (int i = 0; i < right_coupling_elements.size(); i++)
		{
			dynamic_cast<CCoupling*>(right_coupling_elements[i])->SetPlayerObject(player);
			right_coupling_elements[i]->SetPosition(x + BOSS_BBOX_WIDTH - 10, y - BOSS_BBOX_HEIGHT / 2 - i * COUPLING_BBOX_HEIGHT);
			right_coupling_elements[i]->Update(dt, coObjects);
		}
		for (int i = 0; i < left_coupling_elements.size(); i++)
		{
			dynamic_cast<CCoupling*>(left_coupling_elements[i])->SetPlayerObject(player);
			left_coupling_elements[i]->SetPosition(x, y - BOSS_BBOX_HEIGHT / 2 - i * COUPLING_BBOX_HEIGHT);
			left_coupling_elements[i]->Update(dt, coObjects);
		}

	}
	/*if (isStartAllCouplingRight)
	{
		for (int i = 0; i < right_coupling_elements.size(); i++)
		{
			right_coupling_elements[i]->Update(dt, coObjects);
		}
	}
	else
	{
		if (time_moving_coupling >= 4000)
		{
			right_coupling_elements[3]->Update(dt, coObjects);
		}
		if (time_moving_coupling >= 5000)
			right_coupling_elements[2]->Update(dt, coObjects);
		if (time_moving_coupling >= 6000)
			right_coupling_elements[1]->Update(dt, coObjects);
		if (time_moving_coupling >= 7000)
		{
			right_coupling_elements[0]->Update(dt, coObjects);
			isStartAllCouplingRight = true;
		}
	}

	if (isStartAllCouplingLeft)
	{
		for (int i = 0; i < left_coupling_elements.size(); i++)
		{
			left_coupling_elements[i]->Update(dt, coObjects);
		}
	}
	else
	{
		if (time_moving_coupling_left >= 4000)
		{
			left_coupling_elements[3]->Update(dt, coObjects);
		}
		if (time_moving_coupling_left >= 5000)
			left_coupling_elements[2]->Update(dt, coObjects);
		if (time_moving_coupling_left >= 6000)
			left_coupling_elements[1]->Update(dt, coObjects);
		if (time_moving_coupling_left >= 7000)
		{
			left_coupling_elements[0]->Update(dt, coObjects);
			isStartAllCouplingLeft = true;
		}
	}*/

	//Xử lý Pincer
	float _x, _y;
	float _delta_x, _delta_y;
	right_coupling_elements[3]->GetPosition(_x, _y);
	dynamic_cast<CPincer*>(right_pincer)->SetPlayerObject(player);
	right_pincer->SetPosition(_x, _y - COUPLING_BBOX_HEIGHT);
	right_pincer->Update(dt, coObjects);

	left_coupling_elements[3]->GetPosition(_x, _y);
	dynamic_cast<CPincer*>(left_pincer)->SetPlayerObject(player);
	left_pincer->SetPosition(_x, _y - COUPLING_BBOX_HEIGHT);
	left_pincer->Update(dt, coObjects);
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
		case BOSS_STATE_MOVE:
			ani = BOSS_ANI_MOVE_LEFT;
			break;
		case BOSS_STATE_IDLE:
			ani = BOSS_ANI_START;
			break;
		case STATE_ITEM:
			ani = item;
			animation_item_set->at(ani - 1)->Render(x, y);
			break;
		}
		if (isDisplay)
		{
			animation_set->at(ani)->Render(x, y);
			//RenderBoundingBox();
			if(state==BOSS_STATE_IDLE)
			{
				if(animation_set->at(ani)->isFinish)
				{
					SetState(BOSS_STATE_MOVE);
				}
				Sound::getInstance()->Play(SOUND_ID_STARTING_BOSS);
			}
			else
			{
				Sound::getInstance()->Stop(SOUND_ID_STARTING_BOSS);
				Sound::getInstance()->Play(SOUND_ID_BOSS);
				for (int i = 0; i < left_coupling_elements.size(); i++)
				{
					if (left_coupling_elements[i])
					{
						left_coupling_elements[i]->Render();
					}
				}
				for (int i = 0; i < right_coupling_elements.size(); i++)
				{
					if (right_coupling_elements[i])
					{
						right_coupling_elements[i]->Render();
					}
				}
				if (right_pincer)
					right_pincer->Render();
				if (left_pincer)
					left_pincer->Render();
			}
		}
		for (int i = 0; i < list_weapon.size(); i++)
		{
			list_weapon[i]->Render();
		}

	}
	
}

void CBoss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STATE_IDLE:
		vx = 0;
		vy = 0;
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
			CWeapon* weapon = new CWeapon(WEAPON_TYPE_BOSS);// Khởi tạo weapon
			weapon->SetDirection(nx);
			weapon->SetPosition(x + BOSS_BBOX_WIDTH / 2, y);
			weapon->SetState(WEAPON_BOSS_STATE_FLY);
			if (player)
				weapon->SetPlayerObject(player);
			list_weapon.push_back(weapon);
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