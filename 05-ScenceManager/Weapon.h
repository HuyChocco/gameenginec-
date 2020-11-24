#include "GameObject.h"
#include "Game.h"

#define WEAPON_BBOX_WIDTH 24
#define WEAPON_BBOX_HEIGHT 8

#define WEAPON_UP_BBOX_WIDTH 8
#define WEAPON_UP_BBOX_HEIGHT 24

#define WEAPON_CANNONS_BBOX_WIDTH 8
#define WEAPON_CANNONS_BBOX_HEIGHT 16

#define WEAPON_EYEBALL_BBOX_WIDTH 8
#define WEAPON_EYEBALL_BBOX_HEIGHT 16

#define WEAPON_TELEPORTER_BBOX_WIDTH 8
#define WEAPON_TELEPORTER_BBOX_HEIGHT 16


#define WEAPON_FLOATER_BBOX_WIDTH 8
#define WEAPON_FLOATER_BBOX_HEIGHT 16

#define WEAPON_SKULL_BBOX_WIDTH 8
#define WEAPON_SKULL_BBOX_HEIGHT 16


#define WEAPON_BIG_HUMAN_BBOX_WIDTH 4
#define WEAPON_BIG_HUMAN_BBOX_HEIGHT 4

#define WEAPON_STATE_NONE		100 //General
#define WEAPON_ANI_SETS_ID 7

//Player
#define WEAPON_STATE_FIRE		700
#define WEAPON_STATE_FIRE_UP		701
#define WEAPON_STATE_EXPLODE		702
#define WEAPON_STATE_FIRE_DOWN		703
//Enemy_Cannons
#define WEAPON_CANNONS_STATE_FIRE_HORIZONTAL_LEFT		700
#define WEAPON_CANNONS_STATE_FIRE_VERTICAL_UP		701
#define WEAPON_CANNONS_STATE_EXPLODE		702
#define WEAPON_CANNONS_STATE_FIRE_VERTICAL_DOWN		703
#define WEAPON_CANNONS_STATE_FIRE_HORIZONTAL_RIGHT		704
//Enemy Eyeball
#define WEAPON_EYEBALL_STATE_FLY		700
#define WEAPON_EYEBALL_STATE_EXPLODE		701
//Enemy Teleporter
#define WEAPON_TELEPORTER_STATE_FLY		700
#define WEAPON_TELEPORTER_STATE_EXPLODE		701

//Enemy Floater
#define WEAPON_FLOATER_STATE_FLY		700
#define WEAPON_FLOATER_STATE_EXPLODE		701

//Enemy Skull
#define WEAPON_SKULL_STATE_FLY_LEFT		700
#define WEAPON_SKULL_STATE_FLY_RIGHT		701
#define WEAPON_SKULL_STATE_EXPLODE		702

//Big Human
#define WEAPON_BIG_HUMAN_STATE_FLY		800
#define WEAPON_BIG_HUMAN_STATE_EXPLODE		801
#define WEAPON_BIG_HUMAN_STATE_FLY_UP		802
#define WEAPON_BIG_HUMAN_STATE_FLY_DOWN		803

#define WEAPON_ANI_BIG_HUMAN 3
#define WEAPON_ANI_EXPLODE_BIG_HUMAN 4
//Player
#define WEAPON_ANI_FLY_HORIZONTAL 0
#define WEAPON_ANI_FLY_VERTICAL 1
#define WEAPON_ANI_EXPLODE 2

//Enemy_Cannons
#define WEAPON_ANI_ENEMY_CANNONS 5
#define WEAPON_ANI_EXPLODE_ENEMY_CANNONS 6
//Enemy_Eyeball
#define WEAPON_ANI_ENEMY_EYEBALL 7
#define WEAPON_ANI_EXPLODE_ENEMY_EYEBALL 8
//Enemy_Teleporter
#define WEAPON_ANI_ENEMY_TELEPORTER 11
#define WEAPON_ANI_EXPLODE_ENEMY_TELEPORTER 12
//Enemy_Floater
#define WEAPON_ANI_ENEMY_FLOATER 9
#define WEAPON_ANI_EXPLODE_ENEMY_FLOATER 10
//Enemy_Skull
#define WEAPON_ANI_ENEMY_SKULL_LEFT 13
#define WEAPON_ANI_ENEMY_SKULL_RIGHT 14
#define WEAPON_ANI_EXPLODE_ENEMY_SKULL 15
//Player
#define WEAPON_FLY_SPEED 0.3f;

#define WEAPON_CANNONS_FLY_SPEED 0.15f;
#define WEAPON_EYEBALL_FLY_SPEED 0.15f;
#define WEAPON_TELEPORTER_FLY_SPEED 0.15f;
#define WEAPON_FLOATER_FLY_SPEED 0.05f;
#define WEAPON_BIG_HUMAN_FLY_SPEED 0.15f;
#define WEAPON_SKULL_FLY_SPEED 0.1f;

#define TIME_ENABLE_FIRE			1000

#define WEAPON_TYPE_PLAYER			1
#define WEAPON_TYPE_ENEMY_CANNONS			2
#define WEAPON_TYPE_ENEMY_EYEBALL			3
#define WEAPON_TYPE_ENEMY_TELEPORTER			3
#define WEAPON_TYPE_BIG_HUMAN			4
#define WEAPON_TYPE_ENEMY_FLOATER			5
#define WEAPON_TYPE_ENEMY_SKULL			6

class CWeapon : public CGameObject
{
	int dame = 0;
	DWORD timeAttack;
	bool isFlyHorizontalRight;
	bool isFlyHorizontalLeft;
	bool isFlyUp;
	int type_weapon;

	bool isBurning;
	bool isAttacked;
	CGameObject* player;
public:
	CWeapon(int type);
	CWeapon(float x, float y, int nx, int state,bool isBarrelUp);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_object = NULL);
	virtual void Render();

	void SetState(int state);
	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetTypeWeapon(int type) { this->type_weapon = type; }
	int GetTypeWeapon() { return this->type_weapon; }

	int GetDame() { return this->dame; }
	//void SetAttack(bool flag) { isAttacked = flag; }
	void SetPlayerObject(CGameObject* _player) { this->player = _player; }

};