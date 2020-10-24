#include "Floater.h"

CFloater::CFloater() :CEnemyObject()
{
	SetState(FLOATER_STATE_IDLE);
	this->blood = 1;
}

void CFloater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y - FLOATER_BBOX_HEIGHT;
	right = x + FLOATER_BBOX_WIDTH;
	bottom = y ;
}

void CFloater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy -= 0.002f * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != FLOATER_STATE_DIE)
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

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CFloater::Render()
{
	if (state != FLOATER_STATE_DIE)
	{
		int ani = -1;
		if (nx > 0)
			ani = FLOATER_ANI_MOVE_RIGHT;
		else
			ani = FLOATER_ANI_MOVE_LEFT;

		animation_set->at(ani)->Render(x, y);
		//RenderBoundingBox();
	}

}

void CFloater::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLOATER_STATE_IDLE:
		vx = 0;
		break;
	case FLOATER_STATE_MOVE:
		if (nx > 0)
		{
			vx = FLOATER_MOVE_SPEED;
		}

		else
		{
			vx = -FLOATER_MOVE_SPEED;
		}

		break;
	case FLOATER_STATE_DIE:
		break;
	default:
		break;
	}
}