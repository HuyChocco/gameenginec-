#include "Weapon.h"
#include "EnemyObject1.h"
#include "Worm.h"
#include "Spider.h"

#include "Brick.h"
#include "Portal.h"
CWeapon::CWeapon(float x, float y, int nx, int state, bool isBarrelUp)
{
	this->x = x;
	this->y = y;
	this->nx = nx;
	this->timeAttack = 0.0f;
	
	if (isBarrelUp)//determine which state used for animation
		SetState(WEAPON_STATE_FIRE_UP);
	else
		SetState(WEAPON_STATE_FIRE);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(7);
	if(ani_set)
		SetAnimationSet(ani_set);
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_object)
{
	if (state != WEAPON_STATE_NONE)
	{
		
		CGameObject::Update(dt);
		timeAttack += dt;
		if (timeAttack > TIME_ENABLE_FIRE)
		{
			SetState(WEAPON_STATE_NONE);
		}
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		if (state != WEAPON_STATE_NONE)
			CalcPotentialCollisions(colliable_object, coEvents);
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

				if (dynamic_cast<CEnemyObject1*>(e->obj)) // if e->obj is Enemy1 
				{
					CEnemyObject1* enemy1_object = dynamic_cast<CEnemyObject1*>(e->obj);


					if (e->nx != 0)
					{
						enemy1_object->SetState(ENEMY1_STATE_DIE);
						SetState(WEAPON_STATE_NONE);
					}

				}
				else if (dynamic_cast<CWorm*>(e->obj)) // if e->obj is Worm 
				{
					CWorm* worm = dynamic_cast<CWorm*>(e->obj);


					if (e->nx != 0)
					{
						worm->SetState(ENEMY1_STATE_DIE);
						SetState(WEAPON_STATE_NONE);
					}

				}
				else if (dynamic_cast<CSpider*>(e->obj)) // if e->obj is Spider 
				{
					CSpider* spider = dynamic_cast<CSpider*>(e->obj);


					if (e->nx != 0)
					{
						spider->SetState(ENEMY1_STATE_DIE);
						SetState(WEAPON_STATE_NONE);
					}

				}
				else if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Spider 
				{
					//if (e->nx != 0)
					{
						
						SetState(WEAPON_STATE_NONE);
					}

				}
				else if (dynamic_cast<CPortal*>(e->obj)) // if e->obj is Spider 
				{
					//if (e->nx != 0)
					{

						SetState(WEAPON_STATE_NONE);
					}

				}


			}

		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	
}

void CWeapon::Render()
{
	if (state != WEAPON_STATE_NONE)
	{
		int flip = false;
		int ani = -1;
		if (nx > 0)
			flip = true;
		else
			flip = false;
		switch (state)
		{
		case WEAPON_STATE_FIRE:
			ani = WEAPON_ANI_FLY_HORIZONTAL;
			break;
		case WEAPON_STATE_FIRE_UP:
			ani = WEAPON_ANI_FLY_VERTICAL;
			break;
		}
		animation_set->at(ani)->Render(x, y, flip);
		RenderBoundingBox();
	}
	
	
}

void CWeapon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WEAPON_STATE_FIRE:
		if (nx > 0)
		{
			vx = WEAPON_FLY_SPEED;
		}
		else
		{
			vx = -WEAPON_FLY_SPEED;
		}
		break;
	case WEAPON_STATE_FIRE_UP:
		vy = WEAPON_CHARACTER_JUMP_SPEED_Y;
		break;
	}
}

void CWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	
	
	if (state == WEAPON_STATE_FIRE_UP)
	{
		top = y - WEAPON_UP_BBOX_HEIGHT;
		right = x + WEAPON_UP_BBOX_WIDTH;
		bottom = y;
	}
	else
	{
		top = y - WEAPON_BBOX_HEIGHT;
		right = x + WEAPON_BBOX_WIDTH;
		bottom = y ;
	}
}

void CWeapon::Reset()
{

}