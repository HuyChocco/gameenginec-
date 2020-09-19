#include "Brick.h"

CBrick::CBrick(float l, float t, float r, float b)
{
	
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
}


void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}