
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
#include "Spike.h"
#include "Eyeball.h"
#include "Cannon.h"
#include "Floater.h"
#include "Dome.h"
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
	CGame* game = CGame::GetInstance();
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Xử lý di chuyển của các đối tượng enemy theo đối tượng nhân vật chính
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CFloater*>(coObjects->at(i))) {
			CFloater* floater = dynamic_cast<CFloater*>(coObjects->at(i));
			if (floater->GetState() != FLOATER_STATE_DIE)
			{
				float x_enemy, y_enemy;
				floater->GetPosition(x_enemy, y_enemy);
				if (x > x_enemy)
					floater->SetDirection(1);
				else
					floater->SetDirection(-1);
				if (floater->GetBlood() < 0)
					floater->SetState(FLOATER_STATE_DIE);
			}

		}
		if (dynamic_cast<CDome*>(coObjects->at(i))) {
			CDome* dome = dynamic_cast<CDome*>(coObjects->at(i));
			if (dome->GetState() != DOME_STATE_DIE)
			{
				float x_enemy, y_enemy;
				dome->GetPosition(x_enemy, y_enemy);
				if (x > x_enemy)
					dome->SetDirection(1);
				else
					dome->SetDirection(-1);
				dome->SetState(DOME_STATE_MOVE);
				if (dome->GetBlood() < 0)
					dome->SetState(DOME_STATE_DIE);
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
				if (worm->GetBlood() < 0)
					worm->SetState(WORM_STATE_DIE);
			}

		}
		else if (dynamic_cast<CEyeball*>(coObjects->at(i))) {
			CEyeball* eyeball = dynamic_cast<CEyeball*>(coObjects->at(i));
			if (eyeball->GetState() != EYEBALL_STATE_DIE)
			{
				float x_eyeball, y_eyeball;
				eyeball->GetPosition(x_eyeball, y_eyeball);
				if (x > x_eyeball)
					eyeball->SetDirection(1);
				else
					eyeball->SetDirection(-1);
				if (y > y_eyeball)
					eyeball->SetDirectionY(1);//Up
				else
					eyeball->SetDirectionY(-1);//Down
				if (eyeball->GetBlood() < 0)
					eyeball->SetState(EYEBALL_STATE_DIE);
				
			}

		}
	}
	// Simple fall down
	if(level==HUMAN_LEVEL_SMALL)
	{
		vy -= HUMAN_GRAVITY * dt;
	}
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
				if (dynamic_cast<CBrick*>(e->obj)) 
				{
					if (e->ny > 0)
					{
						is_on_ground = true;
					}
				}
				else if (dynamic_cast<CSpike*>(e->obj))
				{
					x += dx;
					y += dy;
				}

			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}
int ani = HUMAN_ANI_BIG_WALKING;
int flip = false;
void CHuman::Render()
{
	if (state == MAIN_CHARACTER_STATE_DIE)
		ani = HUMAN_ANI_DIE;
	else
	{
		if (level == HUMAN_LEVEL_BIG)
		{
			switch (state)
			{
			case MAIN_CHARACTER_STATE_RUN_RIGHT:
				flip = true;
				ani = HUMAN_ANI_BIG_WALKING;
				break;
			case MAIN_CHARACTER_STATE_RUN_LEFT:
				flip = false;
				ani = HUMAN_ANI_BIG_WALKING;
				break;
			case MAIN_CHARACTER_STATE_UP_BARREL:
				ani = HUMAN_ANI_BIG_WALKING_UP;
				break;
			case MAIN_CHARACTER_STATE_DOWN_BARREL:
				ani = HUMAN_ANI_BIG_WALKING_DOWN;
				break;
			}
			if (vx == 0 && vy==0) // Nhân vật đứng yên
			{
				animation_set->at(ani)->isPause = true; //Dừng animation 
				animation_set->at(ani)->Render(x, y, flip); // Vẽ frame đang bị tạm dừng
			}
			else // Nhân vật di chuyển
			{
				animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
				animation_set->at(ani)->Render(x, y, flip);
			}
			RenderBoundingBox();
			return;
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
			else if(vx<0)
			{
				ani = HUMAN_ANI_SMALL_WALKING;
				flip = false;
			}


		}

		int alpha = 255;

		animation_set->at(ani)->Render(x, y, flip);
		
		RenderBoundingBox();
	}
		

	
	
	
}

void CHuman::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		vx = HUMAN_WALKING_SPEED;
		nx = 1;
		isGoingUp = false;
		isGoingDown = false;
		break;
	case MAIN_CHARACTER_STATE_RUN_LEFT:
		vx = -HUMAN_WALKING_SPEED;
		nx = -1;
		isGoingUp = false;
		isGoingDown = false;
		break;
	case MAIN_CHARACTER_STATE_JUMP:
		// TODO: need to check if HUMAN is *current* on a platform before allowing to jump again
		if (level == HUMAN_LEVEL_SMALL)
		{
			if (is_on_ground)
			{
				vy = HUMAN_SMALL_JUMP_SPEED_Y;
				is_on_ground = false;
			}
		}
		isGoingUp = false;
		isGoingDown = false;
		break;
	case MAIN_CHARACTER_STATE_IDLE:
		vx = 0;
		if (level == HUMAN_LEVEL_BIG)
			vy = 0;
		break;
	case MAIN_CHARACTER_STATE_UP_BARREL:
		if (level == HUMAN_LEVEL_BIG)
		{
			isGoingUp = true;
			isGoingDown = false;
			vy = HUMAN_WALKING_SPEED;
		}	
		break;
	case MAIN_CHARACTER_STATE_DOWN_BARREL:
		if (level == HUMAN_LEVEL_BIG)
		{
			isGoingUp = false;
			isGoingDown = true;
			vy = -HUMAN_WALKING_SPEED;
		}	
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

