#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"
#include "Map.h"
class CScene
{
protected:
	CKeyEventHandler * key_handler;
	int id;
	LPCWSTR sceneFilePath;

	//Bổ sung
	float pre_player_x;
	float pre_player_y;

public: 
	CScene(int id, LPCWSTR filePath);

	CKeyEventHandler * GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 
	//Bo sung
	virtual CMap* GetMap() { return NULL; }
	virtual void GetNextMap() {  }

	void SetPrePlayerX(float x) { pre_player_x = x; }
	void SetPrePlayerY(float y) { pre_player_y = y; }

	float GetPrePlayerX() { return pre_player_x; }
	float GetPrePlayerY() { return pre_player_y; }
};
typedef CScene * LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scence; 

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene *s) //:CKeyEventHandler() 
	{ scence = s; }

	
};