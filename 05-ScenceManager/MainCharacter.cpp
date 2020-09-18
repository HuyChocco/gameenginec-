#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "MainCharacter.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

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

	// Simple fall down
	vy += MAIN_CHARACTER_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MAIN_CHARACTER_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	

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

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					
				}
				else if (e->nx != 0)
				{
					
				}
			} // if Goomba
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
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
		vy = -MAIN_CHARACTER_JUMP_SPEED_Y;
		break;
	case MAIN_CHARACTER_STATE_IDLE:
		vx = 0;
		break;
	case MAIN_CHARACTER_STATE_DIE:
		vy = -MAIN_CHARACTER_DIE_DEFLECT_SPEED;
		break;
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

