#include "GameObject.h"

#define WEAPON_BBOX_WIDTH 24
#define WEAPON_BBOX_HEIGHT 8

#define WEAPON_UP_BBOX_WIDTH 8
#define WEAPON_UP_BBOX_HEIGHT 24

#define WEAPON_CANNONS_BBOX_WIDTH 8
#define WEAPON_CANNONS_BBOX_HEIGHT 16

#define WEAPON_STATE_NONE		100 //General

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
//Player
#define WEAPON_ANI_SETS_ID 7
#define WEAPON_ANI_SETS_ID_CANNONS 10
#define WEAPON_ANI_FLY_HORIZONTAL 0
#define WEAPON_ANI_FLY_VERTICAL 1
#define WEAPON_ANI_EXPLODE 2

//Enemy_Cannons
#define WEAPON_ANI_ENEMY_CANNONS 2

//Player
#define WEAPON_FLY_SPEED 0.3f;

#define WEAPON_CANNONS_FLY_SPEED 0.2f;


#define TIME_ENABLE_FIRE			1000

#define WEAPON_TYPE_PLAYER			1
#define WEAPON_TYPE_ENEMY_CANNONS			2
class CWeapon : public CGameObject
{
	int dame = 0;
	DWORD timeAttack;
	bool isFlyHorizontalRight;
	bool isFlyHorizontalLeft;
	bool isFlyUp;
	int type_weapon;
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
};