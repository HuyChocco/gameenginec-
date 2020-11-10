#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "MainCharacter.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Brick.h"

#include "EnemyObject1.h"
#include "Worm.h"
#include "Spider.h"
#include "Eyeball.h"
#include "Floater.h"
#include "Dome.h"
#include "Cannon.h"
#include "Jumper.h"
#include "Teleporter.h"

#define JUMPER_ROUNDING_DISTANCE_X 50
#define JUMPER_ROUNDING_DISTANCE_Y 40
CMainCharacter::CMainCharacter(float x, float y) : CGameObject()
{

	SetState(MAIN_CHARACTER_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

}

void CMainCharacter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// reset untouchable timer if untouchable time has passed
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	//Chỉ xét va chạm cho Main Character khi không phải ở state Human
	if (!Is_Human)
	{
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
			}
			else if (dynamic_cast<CCannon*>(coObjects->at(i))) {
				CCannon* cannon = dynamic_cast<CCannon*>(coObjects->at(i));
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
						jumper->SetState(JUMPER_STATE_JUMP);
				}
				
			}
		}
		
		// Simple fall down
		if (state != MAIN_CHARACTER_STATE_NONE_COLLISION)
			//vy += MAIN_CHARACTER_GRAVITY * dt;
			vy -= MAIN_CHARACTER_GRAVITY * dt;

		coEvents.clear();
		// turn off collision when die 
		if (state != MAIN_CHARACTER_STATE_NONE_COLLISION && MAIN_CHARACTER_STATE_DIE)
			CalcPotentialCollisions(coObjects, coEvents);
		if (isAttacked)
		{
			StartUntouchable();
			isAttacked = false;
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

			// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
			//if (rdx != 0 && rdx!=dx)
			//	x += nx*abs(rdx); 

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
				if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is CBrick 
				{
					//CBrick* brick = dynamic_cast<CBrick*>(e->obj);
					if (e->ny > 0)
					{
						Is_On_Ground = true;
					}
				}
				// Nếu là portal object thì thực hiện chuyển cảnh
				else if (dynamic_cast<CPortal*>(e->obj))
				{
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					//Nếu portal là đối tượng chuyển next scene
					if (p->GetType() == 1)
					{
						CGame::GetInstance()->SetIsNextMap(true);
						CGame::GetInstance()->SetIsPreMap(false);
						CGame::GetInstance()->SetSceneId(p->GetSceneId());
						CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
					}
					//Nếu portal là đối tượng chuyển previous scene
					else
					{
						CGame::GetInstance()->SetIsPreMap(true);
						CGame::GetInstance()->SetIsNextMap(false);
						CGame::GetInstance()->SetSceneId(p->GetSceneId());
						CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
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
						StartUntouchable();
						if (e->ny != 0)
						{
							y -= 2*vyWorm * dt;
						}
						else
							x += dx;
					}
					else
					{
						if (e->ny != 0)
						{
							y -= 2*vy*dt;
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
						StartUntouchable();
						
						if (e->ny != 0)
						{
							y -= 2*vyFloater * dt;
						}
						else
							x += dx;
					}
					else
					{
						if (e->ny < 0)
						{
							y -= 2*vy*dt;
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
						StartUntouchable();
						if (e->ny != 0)
						{
							y -= 2* vyDome * dt;
						}
						else
							x += dx;
					}
					else
					{
						if (e->ny < 0)
						{
							y -= 2*vy*dt;
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
						StartUntouchable();
						if (e->ny != 0)
						{
							y -= 2*vyJumper*dt;
						}
						else
							x += dx;
					}
					else
					{
						if (e->ny < 0)
						{
							y -= 2*vy * dt;
						}
						else
							x += dx;
						jumper->SetState(DOME_STATE_DIE);
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
						StartUntouchable();
						if (e->ny != 0)
						{
							y += vyCannon * dt;
						}
						else
							x += dx;
					}
					else
					{
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
						StartUntouchable();
						if (e->ny != 0)
						{
							y += vyEyeball * dt;
						}
						else
							x += dx;
					}
					else
					{
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
					StartUntouchable();
					if (e->ny != 0)
					{
						y += vyTeleporter * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
					teleporter->SetState(TELEPORTER_STATE_DIE);

				}
			}
		}

		}
	}
	//Update list of weapon objects
	if (list_weapon.size() > 0)
	{
		for (int i = 0; i < list_weapon.size(); i++)
			list_weapon[i]->Update(dt, coObjects);
	}
	//Cập nhật vị trí cho các đối tượng thành phần như bánh xe, cabin, human...
	//Chạy hàm cập nhật của các đối tượng thành phần
	for (int i = 0; i < componentObjects.size(); i++)
	{
		//Chạy hàm cập nhật đối tượng CHuman
		if (Is_Human)
		{
			if (dynamic_cast<CHuman*>(componentObjects[i]))
			{
				CHuman* human_object = dynamic_cast<CHuman*>(componentObjects[i]);
				human_object->SetIsBeingHuman(true);
				human_object->Update(dt, coObjects);
			}
		}
		//Chạy hàm cập nhật cho tất cả đối tượng thành phần, kể cả Human
		else
		{
			if (dynamic_cast<CHuman*>(componentObjects[i]))
			{
				CHuman* human_object = dynamic_cast<CHuman*>(componentObjects[i]);
				human_object->SetIsBeingHuman(false);
			}
			componentObjects[i]->SetPosition(x, y);
			if (!dynamic_cast<CHuman*>(componentObjects[i]))
				componentObjects[i]->Update(dt, coObjects);
		}
		
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
	
}

void CMainCharacter::Render()
{
	int alpha = 255;
	CGame* game = CGame::GetInstance();
	if (!game->GetCurrentScene()->GetTypeScence() == OVER_WORLD)
	{
		animation_set->at(0)->Render(x, y, alpha);
		// Vẽ các đối tượng weapon của nhân vật chính
		{
			if (list_weapon.size() > 0)
			{
				for (int i = 0; i < list_weapon.size(); i++)
					list_weapon[i]->Render();
			}
		}
		//Vẽ các object thành phần của player object
		for (int i = 0; i < componentObjects.size(); i++)
		{
			if (!dynamic_cast<CHuman*>(componentObjects[i]))
				componentObjects[i]->SetUntouchable(untouchable);
			//Chỉ Render đối tượng CHuman
			if (Is_Human)
			{
				componentObjects[i]->Render();
			}
			//Render các đối tượng thành phần không phải CHuman
			else
			{
				if (!dynamic_cast<CHuman*>(componentObjects[i]))
					componentObjects[i]->Render();
			}
		}
		//RenderBoundingBox();

	}
	
	else
	{
		Is_Human = true;
		for (int i = 0; i < componentObjects.size(); i++)
		{
			//Chỉ Render đối tượng CHuman
			if (dynamic_cast<CHuman*>(componentObjects[i]))
				componentObjects[i]->Render();
		}
		if (list_weapon.size() > 0)
		{
			for (int i = 0; i < list_weapon.size(); i++)
				list_weapon[i]->Render();
		}
	}
	
	
	
}

void CMainCharacter::SetState(int state)
{
	CGameObject::SetState(state);
	
	switch (state)
	{
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		vx = MAIN_CHARACTER_RUN_SPEED;
		nx = 1;
		break;
	case MAIN_CHARACTER_STATE_RUN_LEFT:
		vx = -MAIN_CHARACTER_RUN_SPEED;
		nx = -1;
		break;
	case MAIN_CHARACTER_STATE_JUMP:
		// TODO: need to check if MAIN_CHARACTER is *current* on a platform before allowing to jump again
		if (Is_On_Ground)
		{
			vy = MAIN_CHARACTER_JUMP_SPEED_Y;
			Is_On_Ground = false;
		}
		break;
	case MAIN_CHARACTER_STATE_IDLE:
		vx = 0;
		break;
	case MAIN_CHARACTER_STATE_DIE:
		//vy = -MAIN_CHARACTER_DIE_DEFLECT_SPEED;
		break;
	case MAIN_CHARACTER_STATE_UP_BARREL:
		break;
	case MAIN_CHARACTER_STATE_DOWN_BARREL:
		break;
	case MAIN_CHARACTER_STATE_BARREL_FIRE:
		break;
	case MAIN_CHARACTER_STATE_NONE_COLLISION:
		break;
	case MAIN_CHARACTER_STATE_HUMAN:
		vx = 0;
		Is_Human = !Is_Human;
		break;
	default:
		
		break;

	}
	//Cập nhật state, hướng di chuyển, tốc độ cho các đối tượng thành phần theo nhân vật chính
	for (int i = 0; i < componentObjects.size(); i++)
	{
		//Khi đang ở state Human thì chỉ cập nhật state, hướng,.. cho đối tượng CHuman
		if (Is_Human)
		{
			if (dynamic_cast<CHuman*>(componentObjects[i]))
			{
				componentObjects[i]->SetState(state);
			}
		}
		else
		{
			componentObjects[i]->SetState(state);
			componentObjects[i]->SetDirection(nx);
			componentObjects[i]->SetSpeed(vx, vy);
		}
		
		if (state == MAIN_CHARACTER_STATE_BARREL_FIRE)//Nhân vật bắn
		{
			if (Is_Human)
			{
				CWeapon* weapon = new CWeapon(WEAPON_TYPE_BIG_HUMAN);// Khởi tạo weapon theo x,y của human
				float x_human, y_human;
				//Lấy vị trí x, y của đối tượng human
				if (dynamic_cast<CHuman*>(componentObjects[i]))
				{
					dynamic_cast<CHuman*>(componentObjects[i])->GetPosition(x_human, y_human);
					
					if (dynamic_cast<CHuman*>(componentObjects[i])->GetGoingUp())
					{
						weapon->SetPosition(x_human + HUMAN_BIG_BBOX_WIDTH / 2, y_human);
						weapon->SetState(WEAPON_BIG_HUMAN_STATE_FLY_UP);
					}
						
					else if (dynamic_cast<CHuman*>(componentObjects[i])->GetGoingDown())
					{
						weapon->SetPosition(x_human+ HUMAN_BIG_BBOX_WIDTH/2, y_human);
						weapon->SetState(WEAPON_BIG_HUMAN_STATE_FLY_DOWN);
					}
					
					else
					{
						if (dynamic_cast<CHuman*>(componentObjects[i])->GetLevel() == HUMAN_LEVEL_BIG)
						{
							weapon->SetPosition(x_human, y_human - HUMAN_BIG_BBOX_HEIGHT / 2);
							weapon->SetDirection(nx);
							weapon->SetState(WEAPON_BIG_HUMAN_STATE_FLY);
						}
						else
						{
							weapon->SetPosition(x_human, y_human - HUMAN_SMALL_BBOX_HEIGHT / 2);
							weapon->SetDirection(nx);
							weapon->SetState(WEAPON_BIG_HUMAN_STATE_FLY);
						}
						
					}
					list_weapon.push_back(weapon);
				}
				
			}
			else
			{
				if (dynamic_cast<CBarrelObject*>(componentObjects[i]))
				{
					float x_barrel_object, y_barrel_object;
					//Lấy vị trí x, y của đối tượng nòng sóng
					dynamic_cast<CBarrelObject*>(componentObjects[i])->GetPosition(x_barrel_object, y_barrel_object);
					//Nếu nòng sóng đang giơ lên
					if (dynamic_cast<CBarrelObject*>(componentObjects[i])->GetIsBarrelUp() == true)
					{

						CWeapon* weapon = new CWeapon(x_barrel_object, y_barrel_object, nx, state, true);// Khởi tạo weapon theo x,y của barrel
						list_weapon.push_back(weapon);
					}
					else
					{
						CWeapon* weapon = new CWeapon(x_barrel_object, y_barrel_object, nx, state, false);// Khởi tạo weapon theo x,y của barrel
						list_weapon.push_back(weapon);
					}
				}
			}
			
			
		}
	}
	
	

}

void CMainCharacter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y - MAIN_CHARACTER_BBOX_HEIGHT;
	right = x + MAIN_CHARACTER_BBOX_WIDTH;
	bottom = y ;
	
}

/*
	Reset MAIN_CHARACTER status to the beginning state of a scene
*/
void CMainCharacter::Reset()
{

	SetState(MAIN_CHARACTER_STATE_IDLE);
	SetPosition(start_x, start_y);
	Is_Human = false;
}

void CMainCharacter::AddComponentObject(CGameObject* object)
{
	componentObjects.push_back(object);
}

vector<LPGAMEOBJECT> CMainCharacter::GetComponentObjects()
{
	return componentObjects;
}

void CMainCharacter::CollisionItem(int item)
{
	/*
		0: whip item
		1: knife item
		2: axe item
		3: bomerang item
		4: small heart
		5: big heart
		6: fire
	*/
	if (item == 0)
	{
		
	}
	else if (item == 1)
	{
		
	}
	else if (item == 2)
	{
		
	}
	else if (item == 3)
	{
		
	}
	else if (item == 4)
	{
		
	}
	else if (item == 5)
	{
		
	}
	//else
		
}
