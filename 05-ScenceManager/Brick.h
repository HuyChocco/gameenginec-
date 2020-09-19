#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  1280
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
	int width;
	int height;
public:
	CBrick(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};