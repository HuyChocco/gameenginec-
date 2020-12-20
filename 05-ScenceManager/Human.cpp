
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Human.h"
#include "Game.h"

#include "Portal.h"
#include "Brick.h"
#include "Stair.h"

#include "Worm.h"
#include "Spider.h"
#include "EnemyObject1.h"
#include "Spike.h"
#include "Eyeball.h"
#include "Cannon.h"
#include "Floater.h"
#include "Dome.h"
#include "Jumper.h"
#include "Teleporter.h"
#include "Orb.h"
#include "Skull.h"
#include "Boss.h"
#include "Sound.h"
#include "Item.h"
#define JUMPER_ROUNDING_DISTANCE_X 50
#define JUMPER_ROUNDING_DISTANCE_Y 20
#define ORB_ROUNDING_DISTANCE_X 120
#define ORB_ROUNDING_DISTANCE_Y 110
#define SKULL_ROUNDING_DISTANCE_X 2
CHuman::CHuman(float x, float y) : CGameObject()
{
	level = HUMAN_LEVEL_SMALL;

	SetState(HUMAN_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CHuman::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (player != NULL)
	{
		CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
		int power = player_object->GetPower();
		if (power < 0)
			return;
	}
	CGame* game = CGame::GetInstance();
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Xử lý di chuyển của các đối tượng enemy theo đối tượng nhân vật chính
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CFloater*>(coObjects->at(i))) {
			CFloater* floater = dynamic_cast<CFloater*>(coObjects->at(i));
			float x_enemy, y_enemy;
			floater->GetPosition(x_enemy, y_enemy);
			if (x > x_enemy)
				floater->SetDirection(1);
			else
				floater->SetDirection(-1);
			if (isBeingHuman)
				floater->SetPlayerObject(this);
		}
		else if (dynamic_cast<CDome*>(coObjects->at(i))) {
			CDome* dome = dynamic_cast<CDome*>(coObjects->at(i));
			float x_enemy, y_enemy;
			dome->GetPosition(x_enemy, y_enemy);
			if (x > x_enemy)
				dome->SetDirection(1);
			else
				dome->SetDirection(-1);
		}
		else if (dynamic_cast<CWorm*>(coObjects->at(i))) {
			CWorm* worm = dynamic_cast<CWorm*>(coObjects->at(i));

			float x_worm, y_worm;
			worm->GetPosition(x_worm, y_worm);
			if (x > x_worm)
				worm->SetDirection(1);
			else
				worm->SetDirection(-1);
		}
		else if (dynamic_cast<CEyeball*>(coObjects->at(i))) {
			CEyeball* eyeball = dynamic_cast<CEyeball*>(coObjects->at(i));

			float x_eyeball, y_eyeball;
			eyeball->GetPosition(x_eyeball, y_eyeball);
			if (x > x_eyeball)
				eyeball->SetDirection(1);
			else
				eyeball->SetDirection(-1);
			if (y > y_eyeball)
				eyeball->SetDirectionY(1);//Up
			else
				eyeball->SetDirectionY(-1);//Down
			if (isBeingHuman)
				eyeball->SetPlayerObject(this);
		}
		else if (dynamic_cast<CTeleporter*>(coObjects->at(i))) {
			CTeleporter* teleporter = dynamic_cast<CTeleporter*>(coObjects->at(i));

			float x_teleporter, y_teleporter;
			teleporter->GetPosition(x_teleporter, y_teleporter);
			if (x > x_teleporter)
				teleporter->SetDirection(1);
			else
				teleporter->SetDirection(-1);
			if (y > y_teleporter)
				teleporter->SetDirectionY(1);//Up
			else
				teleporter->SetDirectionY(-1);//Down
			if (isBeingHuman)
				teleporter->SetPlayerObject(this);
		}
		else if (dynamic_cast<CCannon*>(coObjects->at(i))) {
			CCannon* cannon = dynamic_cast<CCannon*>(coObjects->at(i));
			if (isBeingHuman)
				cannon->SetPlayerObject(this);
		}
		else if (dynamic_cast<CJumper*>(coObjects->at(i))) {
			CJumper* jumper = dynamic_cast<CJumper*>(coObjects->at(i));

			float x_jumper, y_jumper;
			jumper->GetPosition(x_jumper, y_jumper);
			if (x > x_jumper)
				jumper->SetDirection(1);
			else
				jumper->SetDirection(-1);
			if (jumper->GetState() != STATE_ITEM)
			{
				if (abs(x - x_jumper) < JUMPER_ROUNDING_DISTANCE_X && abs(y - y_jumper) < JUMPER_ROUNDING_DISTANCE_Y)
				{
					jumper->SetState(JUMPER_STATE_JUMP);
					jumper->SetIsJumping(true);
				}

				else
					jumper->SetIsJumping(false);
			}

		}
		else if (dynamic_cast<COrb*>(coObjects->at(i))) {
			COrb* orb = dynamic_cast<COrb*>(coObjects->at(i));
			int type = orb->GetType();
			if (type != 1)
			{
				float x_orb, y_orb;
				orb->GetPosition(x_orb, y_orb);
				if (x > x_orb)
					orb->SetDirection(1);
				else
					orb->SetDirection(-1);
				if (orb->GetState() != STATE_ITEM)
				{
					if (abs(x - x_orb) < ORB_ROUNDING_DISTANCE_X && abs(y - y_orb) < ORB_ROUNDING_DISTANCE_Y)
						orb->SetState(ORB_STATE_ATTACK);
					else orb->SetState(ORB_STATE_IDLE);
				}
			}
		}
		else if (dynamic_cast<CStair*>(coObjects->at(i)))
		{
			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			dynamic_cast<CStair*>(coObjects->at(i))->GetBoundingBox(l2, t2, r2, b2);
			if (isStateClimb)
			{
				if (!CGame::GetInstance()->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2))
					isStateClimb = false;
			}
		}
		else if (dynamic_cast<CSkull*>(coObjects->at(i))) {
			CSkull* skull = dynamic_cast<CSkull*>(coObjects->at(i));
			if (isBeingHuman)
				skull->SetPlayerObject(this);
			float x_skull, y_skull;
			skull->GetPosition(x_skull, y_skull);
			if (x > x_skull)
				skull->SetDirection(1);
			else
				skull->SetDirection(-1);
			if (skull->GetState() != STATE_ITEM)
			{
				if (!skull->isAttacked) {
					if (abs(x - x_skull) > SKULL_ROUNDING_DISTANCE_X) {
						if (x < x_skull)
						{
							skull->SetState(SKULL_STATE_MOVE_LEFT);
						}
						else if (x > x_skull)
						{
							skull->SetState(SKULL_STATE_MOVE_RIGHT);
						}
					}
					else
					{
						skull->time_moving += dt;
						//skull->SetState(SKULL_STATE_MOVE_UP);
						skull->SetState(SKULL_STATE_MOVE_RIGHT_ATTACK);
						skull->SetIsAttack(true);

					}

				}
				else
				{
					if (skull->time_moving < dt) {
						skull->SetState(SKULL_STATE_MOVE_UP);
					}
				}

			}
		}
		if (dynamic_cast<CBoss*>(coObjects->at(i))) {
			CBoss* boss = dynamic_cast<CBoss*>(coObjects->at(i));
			if (isBeingHuman)
				boss->SetPlayerObject(this);
		}
	}
	// Simple fall down
	if (level == HUMAN_LEVEL_SMALL && !isStateClimb)
	{
		vy -= HUMAN_GRAVITY * dt;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != HUMAN_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	if (isAttacked)
	{
		if (untouchable == 0)
		{
			StartUntouchable();
			isAttacked = false;
			if (player != NULL)
			{
				CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
				int power = player_object->GetPower();
				power--;
				player_object->SetPower(power);
				Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
			}
		}
	}
	// reset untouchable timer if untouchable time has passed
	if (untouchable == 1)
	{

		if (GetTickCount() - untouchable_start > MAIN_CHARACTER_UNTOUCHABLE_TIME)
		{
			untouchable = 0;
			untouchable_start = 0;
		}
	}
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


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny > 0)
				{
					is_on_ground = true;
				}
			}
			else if (dynamic_cast<CStair*>(e->obj))
			{
				float l, t, r, b;
				dynamic_cast<CStair*>(e->obj)->GetBoundingBox(l, t, r, b);
				if (isBeingHuman)
				{
					SetState(HUMAN_STATE_CLIMB);
					SetPosition(l + ((r - l) / 2) - 4, y);
				}

			}
			else if (dynamic_cast<CSpike*>(e->obj))
			{
				x += dx;
				y += dy;
				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				b1 = b1 - ((b1 - t1) / 2);
				dynamic_cast<CSpike*>(e->obj)->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
				}
			}
			// Nếu là portal object thì thực hiện chuyển cảnh
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				if (level == HUMAN_LEVEL_BIG)
				{
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					//Nếu portal là đối tượng chuyển next scene
					if (p->GetType() == 1)
					{
						CGame::GetInstance()->SetIsNextMap(true);
						CGame::GetInstance()->SetIsPreMap(false);
						CGame::GetInstance()->SetIsUpMap(false);
						CGame::GetInstance()->SetIsDownMap(false);
						CGame::GetInstance()->SetSceneId(p->GetSceneId());
						CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
					}
					//Nếu portal là đối tượng chuyển up scene
					else if (p->GetType() == 3)
					{
						CGame::GetInstance()->SetIsNextMap(false);
						CGame::GetInstance()->SetIsPreMap(false);
						CGame::GetInstance()->SetIsUpMap(true);
						CGame::GetInstance()->SetIsDownMap(false);
						CGame::GetInstance()->SetSceneId(p->GetSceneId());
						CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
					}
					//Nếu portal là đối tượng chuyển down scene
					else if (p->GetType() == 4)
					{
						CGame::GetInstance()->SetIsNextMap(false);
						CGame::GetInstance()->SetIsPreMap(false);
						CGame::GetInstance()->SetIsUpMap(false);
						CGame::GetInstance()->SetIsDownMap(true);
						CGame::GetInstance()->SetSceneId(p->GetSceneId());
						CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
					}
					//Nếu portal là đối tượng chuyển previous scene
					else
					{
						CGame::GetInstance()->SetIsNextMap(false);
						CGame::GetInstance()->SetIsPreMap(true);
						CGame::GetInstance()->SetIsUpMap(false);
						CGame::GetInstance()->SetIsDownMap(false);
						CGame::GetInstance()->SetSceneId(p->GetSceneId());
						CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
					}
				}
			}
			//Outdoor enemies
			else if (dynamic_cast<CWorm*>(e->obj))
			{
				CWorm* worm = dynamic_cast<CWorm*>(e->obj);
				float vxWorm, vyWorm;
				worm->GetSpeed(vxWorm, vyWorm);
				if (worm->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y -= 2 * vyWorm * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
						int power = player_object->GetPower();
						if (power < 8)
						{
							power++;
							player_object->SetPower(power);
						}
					}
					if (e->ny != 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					worm->SetState(WORM_STATE_DIE);
				}

			}
			else if (dynamic_cast<CFloater*>(e->obj))
			{
				CFloater* floater = dynamic_cast<CFloater*>(e->obj);
				float vxFloater, vyFloater;
				floater->GetSpeed(vxFloater, vyFloater);
				if (floater->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y -= 2 * vyFloater * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					floater->SetState(FLOATER_STATE_DIE);
				}


			}
			else if (dynamic_cast<CDome*>(e->obj))
			{
				CDome* dome = dynamic_cast<CDome*>(e->obj);
				float vxDome, vyDome;
				dome->GetSpeed(vxDome, vyDome);
				if (dome->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y -= 2 * vyDome * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					dome->SetState(DOME_STATE_DIE);
				}


			}
			else if (dynamic_cast<CJumper*>(e->obj))
			{
				CJumper* jumper = dynamic_cast<CJumper*>(e->obj);
				float vxJumper, vyJumper;
				jumper->GetSpeed(vxJumper, vyJumper);
				if (jumper->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y -= 2 * vyJumper * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					jumper->SetState(DOME_STATE_DIE);
				}


			}
			else if (dynamic_cast<COrb*>(e->obj))
			{
				COrb* orb = dynamic_cast<COrb*>(e->obj);
				if (orb->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					float vxOrb, vyOrb;
					orb->GetSpeed(vxOrb, vyOrb);
					if (e->ny != 0)
					{
						y -= 2 * vyOrb * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					orb->SetState(ORB_STATE_DIE);
				}
			}
			else if (dynamic_cast<CSkull*>(e->obj))
			{
				CSkull* skull = dynamic_cast<CSkull*>(e->obj);
				float vxSkull, vySkull;
				skull->GetSpeed(vxSkull, vySkull);
				if (skull->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					skull->SetState(SKULL_STATE_DIE);
				}
			}
			//Indoor enemies
			else if (dynamic_cast<CCannon*>(e->obj))
			{
				CCannon* cannon = dynamic_cast<CCannon*>(e->obj);
				float vxCannon, vyCannon;
				cannon->GetSpeed(vxCannon, vyCannon);
				if (cannon->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
					cannon->SetState(CANNON_STATE_DIE);
				}
			}
			else if (dynamic_cast<CEyeball*>(e->obj))
			{
				CEyeball* eyeball = dynamic_cast<CEyeball*>(e->obj);
				float vxEyeball, vyEyeball;
				eyeball->GetSpeed(vxEyeball, vyEyeball);
				if (eyeball->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
					eyeball->SetState(EYEBALL_STATE_DIE);

				}
			}
			else if (dynamic_cast<CTeleporter*>(e->obj))
			{
				CTeleporter* teleporter = dynamic_cast<CTeleporter*>(e->obj);
				float vxTeleporter, vyTeleporter;
				teleporter->GetSpeed(vxTeleporter, vyTeleporter);
				if (teleporter->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
					teleporter->SetState(TELEPORTER_STATE_DIE);
				}
			}
			//Boss
			else if (dynamic_cast<CBoss*>(e->obj))
			{
				CBoss* boss = dynamic_cast<CBoss*>(e->obj);
				float vxBoss, vyBoss;
				boss->GetSpeed(vxBoss, vyBoss);
				if (boss->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
					boss->SetState(BOSS_STATE_DIE);
				}
			}
			//Item
			else if (dynamic_cast<CItem*>(e->obj))
			{
				Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
				CItem* item = dynamic_cast<CItem*>(e->obj);
				if (item->GetType() == 4) {

					int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
					if (power < 8)
					{
						power++;
						dynamic_cast<CMainCharacter*>(player)->SetPower(power);
					}
				}
				item->SetState(ITEM_STATE_DIE);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
int ani = HUMAN_ANI_BIG_WALKING;
int flip = true;
void CHuman::Render()
{
	if (player != NULL)
	{
		CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
		int power = player_object->GetPower();
		if (power < 0)
			return;
	}
	//if (state == MAIN_CHARACTER_STATE_DIE)
		//ani = HUMAN_ANI_DIE;
	//else
	{
		if (level == HUMAN_LEVEL_BIG)
		{
			int alpha = 255;
			if (untouchable)
				alpha = 128;
			switch (state)
			{
			case MAIN_CHARACTER_STATE_RUN_RIGHT:
				flip = true;
				ani = HUMAN_ANI_BIG_WALKING;
				break;
			case MAIN_CHARACTER_STATE_RUN_LEFT:
				flip = false;
				ani = HUMAN_ANI_BIG_WALKING;
				break;
			case MAIN_CHARACTER_STATE_UP_BARREL:
				ani = HUMAN_ANI_BIG_WALKING_UP;
				break;
			case MAIN_CHARACTER_STATE_DOWN_BARREL:
				if (level == HUMAN_LEVEL_BIG)
					ani = HUMAN_ANI_BIG_WALKING_DOWN;
				break;
			}
			if (vx == 0 && vy == 0) // Nhân vật đứng yên
			{
				animation_set->at(ani)->isPause = true; //Dừng animation 
				animation_set->at(ani)->Render(x, y, flip, alpha); // Vẽ frame đang bị tạm dừng
			}
			else // Nhân vật di chuyển
			{
				animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
				animation_set->at(ani)->Render(x, y, flip, alpha);
			}
			RenderBoundingBox();
			return;
		}
		else if (level == HUMAN_LEVEL_SMALL)
		{
			if (vx == 0)
			{
				if (nx > 0)
				{
					if (isStateCrawl)
					{
						ani = HUMAN_ANI_SMALL_CRAWLING;
						flip = false;
					}
					else
					{
						flip = true;
						ani = HUMAN_ANI_SMALL_IDLE;
					}

				}
				else
				{
					if (isStateCrawl)
					{
						flip = true;
						ani = HUMAN_ANI_SMALL_CRAWLING;
					}
					else if (isStateClimb)
					{
						ani = HUMAN_ANI_SMALL_CLIMBING;
					}
					else
					{
						flip = false;
						ani = HUMAN_ANI_SMALL_IDLE;
					}

				}
				animation_set->at(ani)->isPause = true; //Dừng animation
			}
			else if (vx > 0)
			{
				if (isStateCrawl)
				{
					flip = false;
					ani = HUMAN_ANI_SMALL_CRAWLING;
				}
				else
				{
					flip = true;
					ani = HUMAN_ANI_SMALL_WALKING;
				}
				animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó

			}
			else if (vx < 0)
			{
				if (isStateCrawl)
				{
					flip = true;
					ani = HUMAN_ANI_SMALL_CRAWLING;
				}
				else
				{
					flip = false;
					ani = HUMAN_ANI_SMALL_WALKING;
				}
				animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó	
			}
			if (isStateClimb)
			{
				if (vy != 0)
				{
					ani = HUMAN_ANI_SMALL_CLIMBING;
					animation_set->at(ani)->isPause = false;
				}
				else
				{
					ani = HUMAN_ANI_SMALL_CLIMBING;
					animation_set->at(ani)->isPause = true;
				}
			}
			if (isStateExplosion)
			{
				ani = HUMAN_ANI_SMALL_DYING;
				if (animation_set->at(ani)->isFinish)
					isFininshAnimationDying = true;
			}
		}
		int alpha = 255;
		if (untouchable)
			alpha = 128;
		animation_set->at(ani)->Render(x, y, flip, alpha);
		//RenderBoundingBox();
	}
}

void CHuman::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		//if (!isStateClimb)
	{
		vx = HUMAN_WALKING_SPEED;
		nx = 1;
		isGoingUp = false;
		isGoingDown = false;
	}
	break;
	case MAIN_CHARACTER_STATE_RUN_LEFT:
		//if (!isStateClimb)
	{
		vx = -HUMAN_WALKING_SPEED;
		nx = -1;
		isGoingUp = false;
		isGoingDown = false;
	}
	break;
	case MAIN_CHARACTER_STATE_JUMP:
		// TODO: need to check if HUMAN is *current* on a platform before allowing to jump again
		if (level == HUMAN_LEVEL_SMALL)
		{
			if (is_on_ground)
			{
				vy = HUMAN_SMALL_JUMP_SPEED_Y;
				is_on_ground = false;
				Sound::getInstance()->PlayNew(SOUND_ID_PLAYER_JUMPING);
			}
		}
		isGoingUp = false;
		isGoingDown = false;
		break;
	case MAIN_CHARACTER_STATE_IDLE:
		vx = 0;
		if (level == HUMAN_LEVEL_BIG)
			vy = 0;
		else if (level == HUMAN_LEVEL_SMALL)
		{
			if (isStateClimb)
				vy = 0;
		}

		break;
	case MAIN_CHARACTER_STATE_UP_BARREL:
		if (level == HUMAN_LEVEL_BIG)
		{
			isGoingUp = true;
			isGoingDown = false;
			vy = HUMAN_WALKING_SPEED;
		}
		else if (level == HUMAN_LEVEL_SMALL)
		{
			if (isStateCrawl)
			{
				vy = 0.14f;//Tránh rớt va chạm khi chuyển state từ CRAWL->IDLE
				isStateCrawl = false;
			}
			else if (isStateClimb)
			{
				vy = 0.04f;
			}

		}
		break;
	case MAIN_CHARACTER_STATE_DOWN_BARREL:
		if (isBeingHuman)
		{
			if (level == HUMAN_LEVEL_BIG)
			{
				isGoingUp = false;
				isGoingDown = true;
				vy = -HUMAN_WALKING_SPEED;
			}
			else if (level == HUMAN_LEVEL_SMALL)
			{
				if (!isStateClimb)
					isStateCrawl = true;
				if (isStateClimb)
				{
					vy = -0.04f;
				}
			}
		}

		break;
	case MAIN_CHARACTER_STATE_DIE:
		vx = vy = 0;
		break;
	case MAIN_CHARACTER_STATE_EXPLOSION:
		vx = vy = 0;
		isStateExplosion = true;
		break;
	case HUMAN_STATE_CLIMB:
		isStateClimb = true;
		vy = 0;
		vx = 0;
		break;
	}
}

void CHuman::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x;


	if (level == HUMAN_LEVEL_BIG)
	{
		top = y - HUMAN_BIG_BBOX_HEIGHT;
		right = x + HUMAN_BIG_BBOX_WIDTH;
		bottom = y;
	}
	else
	{
		if (isStateCrawl)
		{
			top = y - HUMAN_SMALL_CRAWL_BBOX_HEIGHT;
			right = x + HUMAN_SMALL_CRAWL_BBOX_WIDTH;
			bottom = y;
		}
		else
		{
			top = y - HUMAN_SMALL_BBOX_HEIGHT;
			right = x + HUMAN_SMALL_BBOX_WIDTH;
			bottom = y;
		}

	}
}

/*
	Reset HUMAN status to the beginning state of a scene
*/
void CHuman::Reset()
{
	SetState(HUMAN_STATE_IDLE);
	SetLevel(HUMAN_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

