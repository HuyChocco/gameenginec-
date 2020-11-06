#pragma once

#include "EnemyObject.h"


using namespace std;
#define TELEPORTER_MOVE_SPEED 0.03f;
#define TELEPORTER_GRAVITY 0.002f;

#define TELEPORTER_BBOX_WIDTH 22
#define TELEPORTER_BBOX_HEIGHT 26

#define TELEPORTER_STATE_UNCLOAK 100
#define TELEPORTER_STATE_ATTACK 101

#define TELEPORTER_STATE_DIE 200

#define TELEPORTER_STATE_IDLE 300

#define TELEPORTER_ANI_SETS_ID 10


#define WEAPON_TYPE_ENEMY_TELEPORTERS			2
class CTeleporter :public CEnemyObject
{
	int isFireVertical;
	vector<CGameObject*> list_weapon;
public:
	CTeleporter(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void LostBlood(int _blood) { this->blood -= _blood; }
};
