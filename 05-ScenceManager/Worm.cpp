#include "Worm.h"

CWorm::CWorm() :CEnemyObject()
{
	SetState(WORM_STATE_IDLE);
}

void CWorm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y- WORM_BBOX_HEIGHT;
	right = x + WORM_BBOX_WIDTH;
	bottom = y ;
}

void CWorm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy -= 0.002f * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != WORM_STATE_DIE)
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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CWorm::Render()
{
	if (state != WORM_STATE_DIE)
	{
		int ani = -1;
		if (nx > 0)
			ani = WORM_ANI_MOVE_RIGHT;
		else
			ani = WORM_ANI_MOVE_LEFT;

		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();
	}

}

void CWorm::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WORM_STATE_IDLE:
		vx = 0;
		break;
	case WORM_STATE_MOVE:
		if (nx > 0)
		{
			vx = WORM_MOVE_SPEED;
		}

		else
		{
			vx = -WORM_MOVE_SPEED;
		}

		break;
	case WORM_STATE_DIE:
		break;
	default:
		break;
	}
}