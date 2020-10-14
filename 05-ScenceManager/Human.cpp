
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Human.h"
#include "Game.h"

#include "Portal.h"
#include "Brick.h"

#include "Worm.h"
#include "Spider.h"
#include "EnemyObject1.h"
CHuman::CHuman(float x, float y) : CGameObject()
{
	level = HUMAN_LEVEL_SMALL;

	SetState(HUMAN_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CHuman::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Xử lý di chuyển của các đối tượng enemy theo đối tượng nhân vật chính
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
	// Simple fall down
	vy -= HUMAN_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != HUMAN_STATE_DIE)
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

		// how to push back HUMAN if collides with a moving objects, what if HUMAN is pushed this way into another object?
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
				if (e->ny > 0)
				{
					is_on_ground = true;
				}
			}
			
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CHuman::Render()
{
	int ani = -1;
	bool flip = false;
	
	if (state == MAIN_CHARACTER_STATE_DIE)
		ani = HUMAN_ANI_DIE;
	else
		if (level == HUMAN_LEVEL_BIG)
		{

		}
		else if (level == HUMAN_LEVEL_SMALL)
		{
			if (vx == 0)
			{

				ani = HUMAN_ANI_SMALL_IDLE;
				if (nx > 0)
					flip = true;
				else
					flip = false;
			}
			else if (vx > 0)
			{
				ani = HUMAN_ANI_SMALL_WALKING;
				flip = true;
			}
			else
			{
				ani = HUMAN_ANI_SMALL_WALKING;
				flip = false;
			}


		}

	int alpha = 255;


	animation_set->at(ani)->Render(x, y, flip, alpha);

	RenderBoundingBox();
	
	
}

void CHuman::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		vx = HUMAN_WALKING_SPEED;
		nx = 1;
		break;
	case MAIN_CHARACTER_STATE_RUN_LEFT:
		vx = -HUMAN_WALKING_SPEED;
		nx = -1;
		break;
	case MAIN_CHARACTER_STATE_JUMP:
		// TODO: need to check if HUMAN is *current* on a platform before allowing to jump again
		
		if (is_on_ground)
		{
			vy = MAIN_CHARACTER_JUMP_SPEED_Y;
			is_on_ground = false;
		}
		break;
	case MAIN_CHARACTER_STATE_IDLE:
		vx = 0;
		break;
	case MAIN_CHARACTER_STATE_DIE:
		break;
	}
}

void CHuman::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x;
	

	if (level == HUMAN_LEVEL_BIG)
	{
		top = y- HUMAN_BIG_BBOX_HEIGHT;
		right = x + HUMAN_BIG_BBOX_WIDTH;
		bottom = y ;
	}
	else
	{
		top = y- HUMAN_SMALL_BBOX_HEIGHT;
		right = x + HUMAN_SMALL_BBOX_WIDTH;
		bottom = y ;
	}
}

/*
	Reset HUMAN status to the beginning state of a scene
*/
void CHuman::Reset()
{
	SetState(HUMAN_STATE_IDLE);
	SetLevel(HUMAN_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

