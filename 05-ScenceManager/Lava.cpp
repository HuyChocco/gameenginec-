#include "LAVA.h"
#include "Game.h"
CLava::CLava(float l, float t, float r, float b)
{

	x = l;

	y = t;

	width = r - l + 1;

	height = -(b - t + 1);

	isDisplay = true;
	isEnable = true;


}

void CLava::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable)
	{

	}
}
void CLava::Render()
{
	int ani = -1;
	if (isEnable)
	{
		RenderBoundingBox();
	}

}
void CLava::SetState(int state)
{
	CGameObject::SetState(state);
}
void CLava::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (isEnable)
	{
		l = x;
		t = y - height;
		r = x + width;
		b = y;
	}

}