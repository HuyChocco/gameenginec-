#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
//#include "TiledMap.h"
#include "Grid.h"


using namespace std;
#define CELL_WIDTH	160
#define CELL_HEIGHT	120
CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
//bổ sung thêm section map
#define SCENE_SECTION_MAP	7
#define SCENE_SECTION_GRID	8

//
#define OBJECT_TYPE_MARIO	0

//Map objects
#define OBJECT_TYPE_BRICK	1

//Enemy objects
#define OBJECT_TYPE_ENEMY1	2
#define OBJECT_TYPE_WORM	3
#define OBJECT_TYPE_SPIDER	10

//Main character objects
#define OBJECT_TYPE_MAIN_CHARACTER	9
#define OBJECT_TYPE_WHEEL_LEFT	4
#define OBJECT_TYPE_WHEEL_RIGHT	5
#define OBJECT_TYPE_WHEEL_MIDDLE	6
#define OBJECT_TYPE_CABIN	7
#define OBJECT_TYPE_BARREL	8


#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int object_id = atoi(tokens[0].c_str());
	int object_type = atoi(tokens[1].c_str());
	float x = atof(tokens[2].c_str());
	float y = atof(tokens[3].c_str());

	int ani_set_id = atoi(tokens[4].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MAIN_CHARACTER:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		
		player = new CMainCharacter();
		player->SetPosition(x, y);
		player->SetAnimationSet(animation_sets->Get(ani_set_id));
		DebugOut(L"[INFO] Player object created!\n");
		return;
		break;
	case OBJECT_TYPE_ENEMY1: obj = new CEnemyObject1(); break;
	case OBJECT_TYPE_WORM: obj = new CWorm(); break;
	case OBJECT_TYPE_SPIDER: obj = new CSpider(); break;
	case OBJECT_TYPE_BRICK: 
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		obj = new CBrick(x, y, r, b);
		break;
	}
	case OBJECT_TYPE_WHEEL_LEFT:
	{
		obj = new CWheelObject();
		obj->SetPosition(x, y);
		obj->SetAnimationSet(animation_sets->Get(ani_set_id));
		obj->SetID(object_id);
		if (player != NULL)
		{

			DebugOut(L"[INFO] MARIO object has been Created Already!\n");
			player->AddComponentObject(obj);
			return;
		}
		break;
	}
	case OBJECT_TYPE_WHEEL_MIDDLE: 
	{
		obj = new CWheelObject();
		CWheelObject* obj_middle_wheel = (CWheelObject*)obj;
		obj_middle_wheel->SetIsMiddleWheel();
		obj->SetPosition(x, y);
		obj->SetID(object_id);
		obj->SetAnimationSet(animation_sets->Get(ani_set_id));
		if (player != NULL)
		{
			DebugOut(L"[INFO] MARIO object has been Created Already!\n");
			player->AddComponentObject(obj);
		}
		return;
		break;
	}
	case OBJECT_TYPE_WHEEL_RIGHT:
	{
		obj = new CWheelObject();
		CWheelObject* obj_right_wheel =(CWheelObject*) obj;
		obj_right_wheel->SetIsRightWheel();
		obj->SetPosition(x, y);
		obj->SetID(object_id);
		obj->SetAnimationSet(animation_sets->Get(ani_set_id));
		if (player != NULL)
		{
			DebugOut(L"[INFO] MARIO object has been Created Already!\n");
			player->AddComponentObject(obj);
		}
		return;
		break;
	}
	case OBJECT_TYPE_CABIN: 
	{
		obj = new CCabinObject();
		obj->SetPosition(x, y);
		obj->SetID(object_id);
		obj->SetAnimationSet(animation_sets->Get(ani_set_id));
		if (player != NULL)
		{
			DebugOut(L"[INFO] MARIO object has been Created Already!\n");
			player->AddComponentObject(obj);
		}
		return;
		break;
	}
	case OBJECT_TYPE_BARREL: 
	{
		obj = new CBarrelObject(); 
		obj->SetPosition(x, y);
		obj->SetID(object_id);
		obj->SetAnimationSet(animation_sets->Get(ani_set_id));
		if (player != NULL)
		{
			DebugOut(L"[INFO] MARIO object has been Created Already!\n");
			player->AddComponentObject(obj);
		}
		return;
		break;
	}
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[5].c_str());
			float b = atof(tokens[6].c_str());
			int scene_id = atoi(tokens[7].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	obj->SetID(object_id);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

/*
	Parse a line in section [MAP]
*/
void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 1) return; // skip invalid lines - an map must have at least path

	wstring path = ToWSTR(tokens[0]);

	CTiledMap::GetInstance()->LoadMap(path.c_str());

}
bool initGridFlag = true;
void CPlayScene::_ParseSection_GRID(string line)
{
	// check flag to init grid
	if (initGridFlag)
		{
		int width, height; 
		CTiledMap::GetInstance()->GetMapWidth(width);
		CTiledMap::GetInstance()->GetMapHeight(height);

		int numCol = width / CELL_WIDTH;
		int numRow = height / CELL_HEIGHT;

		(height % CELL_HEIGHT == 0) ? numRow = numRow : numRow = numRow += 1;
		(width % CELL_HEIGHT == 0) ? numCol = numCol : numCol = numCol += 1;

		CGrid::GetInstance()->SetCol(numCol);
		CGrid::GetInstance()->SetRow(numRow);

		CGrid::GetInstance()->Init();

		initGridFlag = false;
	}

	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines
	else
	{
		int cellX = atoi(tokens[0].c_str());
		int cellY = atoi(tokens[1].c_str());

		for (int i = 2; i < tokens.size(); i++)
		{
			int object_id = atoi(tokens[i].c_str());
			for (int j = 0; j < objects.size(); j++)
			{
				LPGAMEOBJECT object = objects.at(j);
				if (object->GetID() == object_id)
				{
					CGrid::GetInstance()->Add(object, cellX, cellY);
				}
			}
		}
	}
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			//bổ sung
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_GRID:
			{
				_ParseSection_GRID(line);
				break;
			}
		}
	}

	f.close();
	//Texture for bounding box
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	
	// get objects from grid 
	objects = CGrid::GetInstance()->GetList();
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);

	}
	if (player == NULL) return;
	else
	{
		player->Update(dt, &coObjects);
		
	}
	
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	
	CGame* game = CGame::GetInstance();
	if (game->GetIsNextMap() == true)
	{
		// switch scene
		game->SwitchScene(game->GetSceneId());
		game->SetIsNextMap(false);
	}
	// Update camera to follow main character
	float cx = 0, cy = 0;
	if (player != NULL)
	{
		player->GetPosition(cx, cy);
	}

	
	CMap* map = CTiledMap::GetInstance();
	int widthMap, heightMap;
	map->GetMapWidth(widthMap);
	map->GetMapHeight(heightMap);
	if (cx < game->GetScreenWidth() / 2)
	{
		cx = 0;
		cy = 0;
	}
	else if (widthMap - cx < game->GetScreenWidth() / 2)
	{
		cx = widthMap - game->GetScreenWidth();
	}
	else
	{
		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;
	}
	

	CGame::GetInstance()->SetCamPos(cx, 20.0f/*cy*/);
}

void CPlayScene::Render()
{
	CTiledMap::GetInstance()->Render();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	player->Render();
	for (int i = 0; i < player->GetComponentObjects().size(); i++)
		player->GetComponentObjects()[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	
	player = NULL;
	CGrid::GetInstance()->Unload();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMainCharacter *player = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		player->SetState(MAIN_CHARACTER_STATE_JUMP);
		break;
	case DIK_A:
		player->Reset();
		break;
	case DIK_Z:
		player->SetState(MAIN_CHARACTER_STATE_BARREL_FIRE);
		break;
	}
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMainCharacter *player = ((CPlayScene*)scence)->GetPlayer();
	//// disable control key when Mario die 
	if (player->GetState() == MAIN_CHARACTER_STATE_DIE) return;
	
	if (game->IsKeyDown(DIK_UP))
	{
		//DebugOut(L"[INFO] KeyDown: DIK_UP\n");
		player->SetState(MAIN_CHARACTER_STATE_UP_BARREL);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
		player->SetState(MAIN_CHARACTER_STATE_RUN_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		player->SetState(MAIN_CHARACTER_STATE_RUN_LEFT);
	else
	{
		player->SetState(MAIN_CHARACTER_STATE_IDLE);
		//DebugOut(L"[INFO] ELSE\n");
	}
		
}