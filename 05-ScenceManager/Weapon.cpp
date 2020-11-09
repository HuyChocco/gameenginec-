
#include "Weapon.h"

#include "EnemyObject1.h"
#include "Worm.h"
#include "Spider.h"
#include "Cannon.h"
#include "Eyeball.h"
#include "Dome.h"
#include "Floater.h"
#include "Teleporter.h"
#include "Brick.h"
#include "Portal.h"
#include "Spike.h"
#include "Jumper.h"
#include "Orb.h"



CWeapon::CWeapon(int type)
{
	isAttacked = false;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(WEAPON_ANI_SETS_ID);
	if (ani_set)
		SetAnimationSet(ani_set);
	if (type == WEAPON_TYPE_ENEMY_CANNONS)
	{
		this->timeAttack = 0.0f;
		SetTypeWeapon(WEAPON_TYPE_ENEMY_CANNONS);
		
	}
	else if (type == WEAPON_TYPE_ENEMY_TELEPORTER)
	{
		this->timeAttack = 0.0f;
		SetTypeWeapon(WEAPON_TYPE_ENEMY_TELEPORTER);
	}
	else if (type == WEAPON_TYPE_ENEMY_EYEBALL)
	{
		this->timeAttack = 0.0f;
		SetTypeWeapon(WEAPON_TYPE_ENEMY_EYEBALL);
	}
	else if (type == WEAPON_TYPE_BIG_HUMAN)
	{
		this->timeAttack = 0.0f;
		SetTypeWeapon(WEAPON_TYPE_BIG_HUMAN);
	}
	else if (type == WEAPON_TYPE_ENEMY_FLOATER)
	{
		this->timeAttack = 0.0f;
		SetTypeWeapon(WEAPON_TYPE_ENEMY_FLOATER);
	}
}
CWeapon::CWeapon(float x, float y, int nx, int state, bool isBarrelUp)
{
	this->x = x;
	this->y = y;
	this->nx = nx;
	this->timeAttack = 0.0f;
	SetTypeWeapon(WEAPON_TYPE_PLAYER);
	if (isBarrelUp)//determine which state used for animation
		SetState(WEAPON_STATE_FIRE_UP);
	else
		SetState(WEAPON_STATE_FIRE);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(WEAPON_ANI_SETS_ID);
	if(ani_set)
		SetAnimationSet(ani_set);
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_object)
{
	CGame* game = CGame::GetInstance();
	float x_player = 0, y_player = 0;
	float t_player=0, l_player=0, r_player=0, b_player=0;
	if (player)
	{
		player->GetPosition(x_player, y_player);
		player->GetBoundingBox(l_player, t_player, r_player, b_player);
	}
	if (state != WEAPON_STATE_NONE)
	{
		
		CGameObject::Update(dt);
		timeAttack += dt;
		if (timeAttack > TIME_ENABLE_FIRE)
		{
			SetState(WEAPON_STATE_NONE);
		}
		if (!isBurning)
		{
			x += dx;
			y += dy;
		}
		
		if (type_weapon == WEAPON_TYPE_PLAYER)
		{
			
			for (UINT i = 0; i < colliable_object->size(); i++)
			{
				if (dynamic_cast<CWorm*>(colliable_object->at(i)))
				{
					CWorm* worm = dynamic_cast<CWorm*>(colliable_object->at(i));
					if (worm->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						worm->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);
							isBurning = true;

							if (!isAttacked)
							{
								worm->LostBlood(GetDame());
								isAttacked = true;
							}
						}
					}
					
				}
				else if (dynamic_cast<CFloater*>(colliable_object->at(i)))
				{
					CFloater* floater = dynamic_cast<CFloater*>(colliable_object->at(i));
					if (floater->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						floater->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);
							isBurning = true;
							if (!isAttacked)
							{
								floater->LostBlood(GetDame());
								isAttacked = true;
							}
						}
					}
					
				}
				else if (dynamic_cast<CDome*>(colliable_object->at(i)))
				{
					CDome* dome = dynamic_cast<CDome*>(colliable_object->at(i));
					if (dome->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						dome->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);
							isBurning = true;
							if (!isAttacked)
							{
								dome->LostBlood(GetDame());
								isAttacked = true;
							}
						}
					}
					
				}
				else if (dynamic_cast<CJumper*>(colliable_object->at(i)))
				{
					CJumper* jumper = dynamic_cast<CJumper*>(colliable_object->at(i));
					if (jumper->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						jumper->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);
							isBurning = true;
							if (!isAttacked)
							{
								jumper->LostBlood(GetDame());
								isAttacked = true;
							}
						}
					}

				}
				else if (dynamic_cast<COrb*>(colliable_object->at(i)))
				{
					COrb* orb = dynamic_cast<COrb*>(colliable_object->at(i));
					if (orb->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						orb->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);
							isBurning = true;
							if (!isAttacked)
							{
								orb->LostBlood(GetDame());
								isAttacked = true;
							}
						}
					}

				}
				else if (dynamic_cast<CSpike*>(colliable_object->at(i)))
				{


				}
				else if (dynamic_cast<CPortal*>(colliable_object->at(i)))
				{
					CPortal* portal = dynamic_cast<CPortal*>(colliable_object->at(i));
					float l1, t1, r1, b1, l2, t2, r2, b2;
					GetBoundingBox(l1, t1, r1, b1);
					portal->GetBoundingBox(l2, t2, r2, b2);

					if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
					{
						SetState(WEAPON_STATE_EXPLODE);
						isBurning = true;
					}

				}
				else if (dynamic_cast<CBrick*>(colliable_object->at(i)))
				{
					CBrick* brick = dynamic_cast<CBrick*>(colliable_object->at(i));
					float l1, t1, r1, b1, l2, t2, r2, b2;
					GetBoundingBox(l1, t1, r1, b1);
					brick->GetBoundingBox(l2, t2, r2, b2);

					if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
					{
						SetState(WEAPON_STATE_EXPLODE);
						isBurning = true;
					}
				}
			}
			

		}
		else if (type_weapon == WEAPON_TYPE_ENEMY_CANNONS)
		{
			for (UINT i = 0; i < colliable_object->size(); i++)
			{
				if (dynamic_cast<CBrick*>(colliable_object->at(i)))
				{
					CBrick* brick = dynamic_cast<CBrick*>(colliable_object->at(i));
					float l1, t1, r1, b1, l2, t2, r2, b2;
					GetBoundingBox(l1, t1, r1, b1);
					brick->GetBoundingBox(l2, t2, r2, b2);

					if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
					{
						SetState(WEAPON_STATE_EXPLODE);
						isBurning = true;
					}
				}
				else if (dynamic_cast<CPortal*>(colliable_object->at(i)))
				{
					
				}
				else if (dynamic_cast<CSpike*>(colliable_object->at(i)))
				{

				}
			}
		}
		else if (type_weapon == WEAPON_TYPE_ENEMY_EYEBALL)
		{
			for (UINT i = 0; i < colliable_object->size(); i++)
			{
				if (dynamic_cast<CBrick*>(colliable_object->at(i)))
				{
					CBrick* brick = dynamic_cast<CBrick*>(colliable_object->at(i));
					float l1, t1, r1, b1, l2, t2, r2, b2;
					GetBoundingBox(l1, t1, r1, b1);
					brick->GetBoundingBox(l2, t2, r2, b2);

					if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
					{
						SetState(WEAPON_STATE_EXPLODE);
						isBurning = true;
					}
				}
				else if (dynamic_cast<CPortal*>(colliable_object->at(i)))
				{

				}
				else if (dynamic_cast<CSpike*>(colliable_object->at(i)))
				{

				}
			}
		}
		else if (type_weapon == WEAPON_TYPE_ENEMY_TELEPORTER)
		{
		for (UINT i = 0; i < colliable_object->size(); i++)
		{
			if (dynamic_cast<CBrick*>(colliable_object->at(i)))
			{
				CBrick* brick = dynamic_cast<CBrick*>(colliable_object->at(i));
				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				brick->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					SetState(WEAPON_STATE_EXPLODE);
					isBurning = true;
				}
			}
			else if (dynamic_cast<CPortal*>(colliable_object->at(i)))
			{

			}
			else if (dynamic_cast<CSpike*>(colliable_object->at(i)))
			{

			}
		}
		}
		else if (type_weapon == WEAPON_TYPE_BIG_HUMAN)
		{
		for (UINT i = 0; i < colliable_object->size(); i++)
			{
				if (dynamic_cast<CBrick*>(colliable_object->at(i)))
				{
					CBrick* brick = dynamic_cast<CBrick*>(colliable_object->at(i));
					float l1, t1, r1, b1, l2, t2, r2, b2;
					GetBoundingBox(l1, t1, r1, b1);
					brick->GetBoundingBox(l2, t2, r2, b2);

					if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
					{
						SetState(WEAPON_STATE_EXPLODE);
						isBurning = true;
					}
				}
				else if (dynamic_cast<CPortal*>(colliable_object->at(i)))
				{
					CPortal* portal = dynamic_cast<CPortal*>(colliable_object->at(i));
					float l1, t1, r1, b1, l2, t2, r2, b2;
					GetBoundingBox(l1, t1, r1, b1);
					portal->GetBoundingBox(l2, t2, r2, b2);

					if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
					{
						SetState(WEAPON_STATE_EXPLODE);
						isBurning = true;
					}

				}
				else if (dynamic_cast<CFloater*>(colliable_object->at(i)))
				{
					CFloater* floater = dynamic_cast<CFloater*>(colliable_object->at(i));
					if (floater->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						floater->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);
							isBurning = true;
							if (!isAttacked)
							{
								floater->LostBlood(GetDame());
								isAttacked = true;
							}
						}
					}
					
				}
				else if (dynamic_cast<CDome*>(colliable_object->at(i)))
				{
					CDome* dome = dynamic_cast<CDome*>(colliable_object->at(i));
					if (dome->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						dome->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);
							isBurning = true;
							if (!isAttacked)
							{
								dome->LostBlood(GetDame());
								isAttacked = true;
							}
						}
					}
					
				}
				else if (dynamic_cast<CJumper*>(colliable_object->at(i)))
				{
					CJumper* jumper = dynamic_cast<CJumper*>(colliable_object->at(i));
					if (jumper->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						jumper->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);
							isBurning = true;
							if (!isAttacked)
							{
								jumper->LostBlood(GetDame());
								isAttacked = true;
							}
						}
					}

				}
				else if (dynamic_cast<COrb*>(colliable_object->at(i)))
				{
					COrb* orb = dynamic_cast<COrb*>(colliable_object->at(i));
					if (orb->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						orb->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);
							isBurning = true;
							if (!isAttacked)
							{
								orb->LostBlood(GetDame());
								isAttacked = true;
							}
						}
					}

				}
				else if (dynamic_cast<CWorm*>(colliable_object->at(i)))
				{
				CWorm* worm = dynamic_cast<CWorm*>(colliable_object->at(i));
				if (worm->GetState() != STATE_ITEM)
				{
					float l1, t1, r1, b1, l2, t2, r2, b2;
					GetBoundingBox(l1, t1, r1, b1);
					worm->GetBoundingBox(l2, t2, r2, b2);

					if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
					{
						SetState(WEAPON_STATE_EXPLODE);
						isBurning = true;
						if (!isAttacked)
						{
							worm->LostBlood(GetDame());
							isAttacked = true;
						}
					}
				}

				}
				else if (dynamic_cast<CCannon*>(colliable_object->at(i)))
				{
				CCannon* cannon = dynamic_cast<CCannon*>(colliable_object->at(i));
				if (cannon->GetState() != STATE_ITEM)
				{
					float l1, t1, r1, b1, l2, t2, r2, b2;
					GetBoundingBox(l1, t1, r1, b1);
					cannon->GetBoundingBox(l2, t2, r2, b2);

					if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
					{
						SetState(WEAPON_STATE_EXPLODE);
						isBurning = true;
						if (!isAttacked)
						{
							cannon->LostBlood(GetDame());
							isAttacked = true;
						}
					}
				}

				}
				else if (dynamic_cast<CEyeball*>(colliable_object->at(i)))
				{
					CEyeball* eyeball = dynamic_cast<CEyeball*>(colliable_object->at(i));
					if (eyeball->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						eyeball->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);

							isBurning = true;
							if (!isAttacked)
							{
								eyeball->LostBlood(GetDame());
								isAttacked = true;
							}

						}
					}


				}
				else if (dynamic_cast<CTeleporter*>(colliable_object->at(i)))
				{
					CTeleporter* tele = dynamic_cast<CTeleporter*>(colliable_object->at(i));
					if (tele->GetState() != STATE_ITEM)
					{
						float l1, t1, r1, b1, l2, t2, r2, b2;
						GetBoundingBox(l1, t1, r1, b1);
						tele->GetBoundingBox(l2, t2, r2, b2);

						if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
						{
							SetState(WEAPON_STATE_EXPLODE);

							isBurning = true;
							if (!isAttacked)
							{
								tele->LostBlood(GetDame());
								isAttacked = true;
							}

						}
					}

				}
			}
		}
		else if (type_weapon == WEAPON_TYPE_ENEMY_FLOATER)
		{
			if (x_player >= this->x)
				x++;
			else
				x--;
			if (y_player >= this->y)
				y++;
			else
				y--;
			float l1, t1, r1, b1;
			GetBoundingBox(l1, t1, r1, b1);
			if (game->CheckCollision(l1, t1, r1, b1, l_player, t_player, r_player, b_player) == true)
			{
				SetState(WEAPON_STATE_EXPLODE);
				isBurning = true;
				player->SetIsAttacked(true);
			}
			for (UINT i = 0; i < colliable_object->size(); i++)
			{
				if (dynamic_cast<CBrick*>(colliable_object->at(i)))
				{
					CBrick* brick = dynamic_cast<CBrick*>(colliable_object->at(i));
					float l1, t1, r1, b1, l2, t2, r2, b2;
					GetBoundingBox(l1, t1, r1, b1);
					brick->GetBoundingBox(l2, t2, r2, b2);

					if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
					{
						SetState(WEAPON_STATE_EXPLODE);
						isBurning = true;
					}
				}
				else if (dynamic_cast<CPortal*>(colliable_object->at(i)))
				{

				}
				else if (dynamic_cast<CSpike*>(colliable_object->at(i)))
				{

				}
			}
		}

			
	}

		
		
	
}

void CWeapon::Render()
{
	int typeWeapon = GetTypeWeapon();
	if (typeWeapon == WEAPON_TYPE_PLAYER)
	{
		if (state != WEAPON_STATE_NONE)
		{
			int flip = false;
			int ani = -1;
			if (nx > 0)
				flip = true;
			else
				flip = false;
			switch (state)
			{
			case WEAPON_STATE_FIRE:
				ani = WEAPON_ANI_FLY_HORIZONTAL;
				break;
			case WEAPON_STATE_FIRE_UP:
				ani = WEAPON_ANI_FLY_VERTICAL;
				break;
			case WEAPON_STATE_EXPLODE:
				ani = WEAPON_ANI_EXPLODE;
				break;
			}
			if (state == WEAPON_STATE_EXPLODE)
			{
				float l, t, r, b;
				GetBoundingBox(l, t, r, b);
				if (isFlyHorizontalRight)
					animation_set->at(ani)->Render(r - 10, y, flip);
				else if (isFlyHorizontalLeft)
					animation_set->at(ani)->Render(l - 10, y, flip);
				else
					animation_set->at(ani)->Render(x, b + 10, flip);
				if (animation_set->at(ani)->isFinish)
				{
					animation_set->at(ani)->isFinish = false;
					SetState(WEAPON_STATE_NONE);
				}
			}
			else
				animation_set->at(ani)->Render(x, y, flip);
			//RenderBoundingBox();
		}
	}
	else if (typeWeapon == WEAPON_TYPE_ENEMY_CANNONS)
	{
		if (state != WEAPON_STATE_NONE)
		{
			int ani = WEAPON_ANI_ENEMY_CANNONS;
			int flip = false;
			switch (state)
			{
			case WEAPON_STATE_EXPLODE:
				ani = WEAPON_ANI_EXPLODE_ENEMY_CANNONS;
				break;
			case WEAPON_CANNONS_STATE_FIRE_HORIZONTAL_RIGHT:
				flip = true;
			default:
				flip = false;
			}
			if (state == WEAPON_STATE_EXPLODE)
			{
				float l, t, r, b;
				GetBoundingBox(l, t, r, b);
				animation_set->at(ani)->Render(x, y, flip);
				if (animation_set->at(ani)->isFinish)
				{
					animation_set->at(ani)->isFinish = false;
					SetState(WEAPON_STATE_NONE);
				}
			}
			else
				animation_set->at(ani)->Render(x, y, flip);
			//RenderBoundingBox();
		}
	}
	else if (typeWeapon == WEAPON_TYPE_ENEMY_EYEBALL)
	{
		if (state != WEAPON_STATE_NONE)
		{
			int ani = WEAPON_ANI_ENEMY_EYEBALL;
			int flip = false;
			switch (state)
			{
			case WEAPON_EYEBALL_STATE_FLY:
				ani = WEAPON_ANI_ENEMY_EYEBALL;
				break;
			case WEAPON_STATE_EXPLODE:
				ani = WEAPON_ANI_EXPLODE_ENEMY_EYEBALL;
				break;
			default:
				break;
			}
			if (state == WEAPON_STATE_EXPLODE)
			{
				float l, t, r, b;
				GetBoundingBox(l, t, r, b);
				animation_set->at(ani)->Render(x, y, flip);
				if (animation_set->at(ani)->isFinish)
				{
					animation_set->at(ani)->isFinish = false;
					SetState(WEAPON_STATE_NONE);
				}
			}
			else
				animation_set->at(ani)->Render(x, y, flip);
			//RenderBoundingBox();
		}
	}
	else if (typeWeapon == WEAPON_TYPE_ENEMY_TELEPORTER)
	{
	if (state != WEAPON_STATE_NONE)
	{
		int ani = WEAPON_ANI_ENEMY_TELEPORTER;
		int flip = false;
		switch (state)
		{
		case WEAPON_EYEBALL_STATE_FLY:
			ani = WEAPON_ANI_ENEMY_TELEPORTER;
			break;
		case WEAPON_STATE_EXPLODE:
			ani = WEAPON_ANI_EXPLODE_ENEMY_TELEPORTER;
			break;
		default:
			break;
		}
		if (state == WEAPON_STATE_EXPLODE)
		{
			float l, t, r, b;
			GetBoundingBox(l, t, r, b);
			animation_set->at(ani)->Render(x, y, flip);
			if (animation_set->at(ani)->isFinish)
			{
				animation_set->at(ani)->isFinish = false;
				SetState(WEAPON_STATE_NONE);
			}
		}
		else
			animation_set->at(ani)->Render(x, y, flip);
		//RenderBoundingBox();
	}
	}
	else if (typeWeapon == WEAPON_TYPE_ENEMY_FLOATER)
	{
		if (state != WEAPON_STATE_NONE)
		{
			int ani = WEAPON_ANI_ENEMY_FLOATER;
			int flip = false;
			switch (state)
			{
			case WEAPON_FLOATER_STATE_FLY:
				ani = WEAPON_ANI_ENEMY_FLOATER;
				break;
			case WEAPON_STATE_EXPLODE:
				ani = WEAPON_ANI_EXPLODE_ENEMY_FLOATER;
				break;
			default:
				break;
			}
			if (state == WEAPON_STATE_EXPLODE)
			{
				float l, t, r, b;
				GetBoundingBox(l, t, r, b);
				animation_set->at(ani)->Render(x, y, flip);
				if (animation_set->at(ani)->isFinish)
				{
					animation_set->at(ani)->isFinish = false;
					SetState(WEAPON_STATE_NONE);
				}
			}
			else
				animation_set->at(ani)->Render(x, y, flip);
			//RenderBoundingBox();
		}
	}
	else if (typeWeapon == WEAPON_TYPE_BIG_HUMAN)
	{
		if (state != WEAPON_STATE_NONE)
		{
			int ani = WEAPON_ANI_BIG_HUMAN;
			int flip = false;
			switch (state)
			{
			case WEAPON_BIG_HUMAN_STATE_FLY:
				ani = WEAPON_ANI_BIG_HUMAN;
				break;

			case WEAPON_STATE_EXPLODE:
				ani = WEAPON_ANI_EXPLODE_BIG_HUMAN;
				break;
			default:
				break;
			}
			if (state == WEAPON_STATE_EXPLODE)
			{
				float l, t, r, b;
				GetBoundingBox(l, t, r, b);

				animation_set->at(ani)->Render(x, y, flip);

				if (animation_set->at(ani)->isFinish)
				{
					animation_set->at(ani)->isFinish = false;
					SetState(WEAPON_STATE_NONE);
				}
			}
			else
				animation_set->at(ani)->Render(x, y, flip);
			//RenderBoundingBox();
		}
	}
}

void CWeapon::SetState(int state)
{
	CGameObject::SetState(state);
	if (type_weapon == WEAPON_TYPE_PLAYER)
	{
		switch (state)
		{
		case WEAPON_STATE_FIRE:
			if (nx > 0)
			{
				vx = WEAPON_FLY_SPEED;
				isFlyHorizontalRight = true;
				isFlyHorizontalLeft = false;
				isFlyUp = false;
			}
			else
			{
				vx = -WEAPON_FLY_SPEED;
				isFlyHorizontalLeft = true;
				isFlyHorizontalRight = false;
			}
			this->dame = 1;
			break;
		case WEAPON_STATE_FIRE_UP:
			isFlyUp = true;
			isFlyHorizontalLeft = false;
			isFlyHorizontalRight = false;
			vy = WEAPON_FLY_SPEED;
			this->dame = 1;
			break;
		case WEAPON_STATE_EXPLODE:
			break;
		}
	}
	else if (type_weapon == WEAPON_TYPE_ENEMY_CANNONS)
	{
		switch (state)
		{
		case WEAPON_CANNONS_STATE_FIRE_HORIZONTAL_LEFT:
			vx = -WEAPON_CANNONS_FLY_SPEED;
			this->dame = 1;
			break;
		case WEAPON_CANNONS_STATE_FIRE_HORIZONTAL_RIGHT:
			vx = WEAPON_CANNONS_FLY_SPEED;
			this->dame = 1;
			break;
		case WEAPON_CANNONS_STATE_FIRE_VERTICAL_UP:
			vy = WEAPON_CANNONS_FLY_SPEED;
			this->dame = 1;
			break;
		case WEAPON_CANNONS_STATE_FIRE_VERTICAL_DOWN:
			vy = -WEAPON_CANNONS_FLY_SPEED;
			this->dame = 1;
			break;
		case WEAPON_CANNONS_STATE_EXPLODE:
			break;
		}
	}
	else if (type_weapon == WEAPON_TYPE_ENEMY_EYEBALL)
	{
		switch (state)
		{
		case WEAPON_EYEBALL_STATE_FLY:
			vx = WEAPON_EYEBALL_FLY_SPEED;
			this->dame = 1;
			break;
		}
	}
	else if (type_weapon == WEAPON_TYPE_ENEMY_TELEPORTER)
	{
		switch (state)
		{
		case WEAPON_EYEBALL_STATE_FLY:
			vx = WEAPON_EYEBALL_FLY_SPEED;
			this->dame = 1;
			break;
		}
	}
	else if (type_weapon == WEAPON_TYPE_BIG_HUMAN)
	{
		switch (state)
		{
		case WEAPON_BIG_HUMAN_STATE_FLY:
			{
				if (nx > 0)
				{
					vx = WEAPON_BIG_HUMAN_FLY_SPEED;
				}
				else
					vx = - WEAPON_BIG_HUMAN_FLY_SPEED;
					this->dame = 1;
			}
			break;
		case WEAPON_BIG_HUMAN_STATE_FLY_UP:
		{
			vy = WEAPON_BIG_HUMAN_FLY_SPEED;

			this->dame = 1;
		}
		break;
		case WEAPON_BIG_HUMAN_STATE_FLY_DOWN:
		{
			vy = - WEAPON_BIG_HUMAN_FLY_SPEED;
			this->dame = 1;
		}

		break;
		}
	}
	else if (type_weapon == WEAPON_TYPE_ENEMY_FLOATER)
	{
		switch (state)
		{
			case WEAPON_FLOATER_STATE_FLY:
			{
				if (nx > 0)
				{
					vx = WEAPON_FLOATER_FLY_SPEED;
				}
				else
				{
					vx = -WEAPON_FLOATER_FLY_SPEED;
				}
				this->dame = 1;
			}
			break;
		}
	}
}

void CWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
	int typeWeapon = GetTypeWeapon();
	if (typeWeapon == WEAPON_TYPE_PLAYER)
	{
		left = x;
		if (state == WEAPON_STATE_FIRE_UP)
		{
			top = y - WEAPON_UP_BBOX_HEIGHT;
			right = x + WEAPON_UP_BBOX_WIDTH;
			bottom = y;
		}
		else
		{
			top = y - WEAPON_BBOX_HEIGHT;
			right = x + WEAPON_BBOX_WIDTH;
			bottom = y;
		}
	}
	else if (typeWeapon == WEAPON_TYPE_ENEMY_CANNONS)
	{
		left = x;
		top = y - WEAPON_CANNONS_BBOX_HEIGHT;
		right = x + WEAPON_CANNONS_BBOX_WIDTH;
		bottom = y;
	}
	else if (typeWeapon == WEAPON_TYPE_ENEMY_EYEBALL)
	{
		left = x;
		top = y - WEAPON_EYEBALL_BBOX_HEIGHT;
		right = x + WEAPON_EYEBALL_BBOX_WIDTH;
		bottom = y;
	}
	else if (typeWeapon == WEAPON_TYPE_ENEMY_TELEPORTER)
	{
		left = x;
		top = y - WEAPON_TELEPORTER_BBOX_HEIGHT;
		right = x + WEAPON_TELEPORTER_BBOX_WIDTH;
		bottom = y;
	}
	else if (typeWeapon == WEAPON_TYPE_ENEMY_FLOATER)
	{
		left = x;
		top = y - WEAPON_FLOATER_BBOX_HEIGHT;
		right = x + WEAPON_FLOATER_BBOX_WIDTH;
		bottom = y;
	}
	else if (typeWeapon == WEAPON_TYPE_BIG_HUMAN)
	{
		left = x;
		top = y - WEAPON_BIG_HUMAN_BBOX_HEIGHT;
		right = x + WEAPON_BIG_HUMAN_BBOX_WIDTH;
		bottom = y;
	}
}

void CWeapon::Reset()
{

}