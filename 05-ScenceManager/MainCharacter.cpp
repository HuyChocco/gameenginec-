#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "MainCharacter.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Brick.h"

#include "EnemyObject1.h"
#include "Worm.h"
#include "Spider.h"
CMainCharacter::CMainCharacter(float x, float y) : CGameObject()
{

	SetState(MAIN_CHARACTER_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

}

void CMainCharacter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	// Calculate dx, dy 
	CGameObject::Update(dt);
	//Render list of weapon objects
	if (list_weapon.size() > 0)
	{
		for (int i = 0; i < list_weapon.size(); i++)
			list_weapon[i]->Update(dt, coObjects);
	}
	// Simple fall down
	vy += MAIN_CHARACTER_GRAVITY * dt;
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MAIN_CHARACTER_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CEnemyObject1*>(coObjects->at(i))) {
			CEnemyObject1* enemy1_object = dynamic_cast<CEnemyObject1*>(coObjects->at(i));
			if (enemy1_object->GetState() != ENEMY1_STATE_DIE)
			{
				float x_enemy, y_enemy;
				enemy1_object->GetPosition(x_enemy, y_enemy);
				if (x > x_enemy)
					enemy1_object->SetDirection(1);
				else
					enemy1_object->SetDirection(-1);
				enemy1_object->SetState(ENEMY1_STATE_WALKING);
			}
			
		}
		else if (dynamic_cast<CWorm*>(coObjects->at(i))) {
			CWorm* worm = dynamic_cast<CWorm*>(coObjects->at(i));
			if (worm->GetState() != WORM_STATE_DIE)
			{
				float x_worm, y_worm;
				worm->GetPosition(x_worm, y_worm);
				if (x > x_worm)
					worm->SetDirection(1);
				else
					worm->SetDirection(-1);
				worm->SetState(WORM_STATE_MOVE);
			}

		}
		else if (dynamic_cast<CSpider*>(coObjects->at(i))) {
			CSpider* spider = dynamic_cast<CSpider*>(coObjects->at(i));
			if (spider->GetState() != SPIDER_STATE_DIE)
			{
				float x_spider, y_spider;
				spider->GetPosition(x_spider, y_spider);
				if (x > x_spider)
					spider->SetDirection(1);
				else
					spider->SetDirection(-1);
				spider->SetState(SPIDER_STATE_MOVE);
			}

		}
	}
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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is CBrick 
			{
				//CBrick* brick = dynamic_cast<CBrick*>(e->obj);

				
				if (e->ny < 0)
				{
					Is_On_Ground = true;
				}
			}
		}
		
	}
	
	for (int i = 0; i < componentObjects.size(); i++)
		componentObjects[i]->SetPosition(x,y);
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}

void CMainCharacter::Render()
{
	
	int alpha = 255;
	animation_set->at(0)->Render(x, y, alpha);
	RenderBoundingBox();
	
	if (list_weapon.size() > 0)
	{
		for (int i = 0; i < list_weapon.size(); i++)
			list_weapon[i]->Render();
	}
}

void CMainCharacter::SetState(int state)
{
	CGameObject::SetState(state);
	
	switch (state)
	{
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		vx = MAIN_CHARACTER_RUN_SPEED;
		nx = 1;
		break;
	case MAIN_CHARACTER_STATE_RUN_LEFT:
		vx = -MAIN_CHARACTER_RUN_SPEED;
		nx = -1;
		break;
	case MAIN_CHARACTER_STATE_JUMP:
		// TODO: need to check if MAIN_CHARACTER is *current* on a platform before allowing to jump again
		if (Is_On_Ground)

		{
			vy = -MAIN_CHARACTER_JUMP_SPEED_Y;
			Is_On_Ground = false;
		}
		
		break;
	case MAIN_CHARACTER_STATE_IDLE:
		vx = 0;
		
		break;
	case MAIN_CHARACTER_STATE_DIE:
		vy = -MAIN_CHARACTER_DIE_DEFLECT_SPEED;
		break;
	case MAIN_CHARACTER_STATE_UP_BARREL:
		break;
	case MAIN_CHARACTER_STATE_BARREL_FIRE:
		break;
	default:
		
		break;

	}
	for (int i = 0; i < componentObjects.size(); i++)
	{
		componentObjects[i]->SetState(state);
		componentObjects[i]->SetDirection(nx);
		componentObjects[i]->SetSpeed(vx,vy);
		if (state == MAIN_CHARACTER_STATE_BARREL_FIRE)//Nhân vật bắn
		{
			if (dynamic_cast<CBarrelObject*>(componentObjects[i]))
			{
				float x_barrel_object, y_barrel_object;
				dynamic_cast<CBarrelObject*>(componentObjects[i])->GetPosition(x_barrel_object, y_barrel_object);
				if (dynamic_cast<CBarrelObject*>(componentObjects[i])->GetIsBarrelUp() == true)
				{
					
					CWeapon* weapon = new CWeapon(x_barrel_object, y_barrel_object, nx, state, true);// Khởi tạo weapon theo x,y của barrel
					list_weapon.push_back(weapon);
				}
				else
				{
					CWeapon* weapon = new CWeapon(x_barrel_object, y_barrel_object, nx, state, false);// Khởi tạo weapon theo x,y của barrel
					list_weapon.push_back(weapon);
				}
			}
		}
	}
	
	

}

void CMainCharacter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MAIN_CHARACTER_BBOX_WIDTH;
	bottom = y + MAIN_CHARACTER_BBOX_HEIGHT;
	
}

/*
	Reset MAIN_CHARACTER status to the beginning state of a scene
*/
void CMainCharacter::Reset()
{
	
}

void CMainCharacter::AddComponentObject(CGameObject* object)
{
	componentObjects.push_back(object);
}

vector<LPGAMEOBJECT> CMainCharacter::GetComponentObjects()
{
	return componentObjects;
}

