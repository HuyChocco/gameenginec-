#include "Grid.h"
#define CELL_WIDTH	160
#define CELL_HEIGHT	120
CGrid* CGrid::__instance = NULL;
CGrid* CGrid::GetInstance()
{
	if (__instance == NULL) __instance = new CGrid();
	return __instance;
}
CGrid::CGrid()
{
	objects.clear();
}

void CGrid::Init()
{
	cells = new CCell* [numCol];
	for (int i = 0; i < numCol; i++)
		cells[i] = new CCell[numRow];
}

void CGrid::Add(LPGAMEOBJECT object, float x, float y)
{
	/*int startX = x / CELL_WIDTH;
	int startY = y / CELL_HEIGHT;
	int endX = (x + object->GetWidth()) / CELL_WIDTH;
	int endY = (y + object->GetHeight()) / CELL_HEIGHT;*/

	//for (int i = startX; i <= endX; i++)
	//{
		/*for (int j = startY; j <= endY; j++)
		{
			cells[i][j].Add(object);
		}*/
		//cells[i][startY].Add(object);
	//}
	//cells[startX][startY].Add(object);
}

void CGrid::Add(LPGAMEOBJECT object, int xCell, int yCell)
{
	cells[xCell][yCell].Add(object);
}

vector<LPGAMEOBJECT> CGrid::GetList()
{
	CGame* game = CGame::GetInstance();

	float x, y;
	game->GetCamPos(x,y);
	

	int startX = x / CELL_WIDTH;
	int endX = (x + game->GetScreenWidth()) / CELL_WIDTH;

	int startY = y / CELL_HEIGHT;
	int endY = (y + game->GetScreenHeight()) / CELL_HEIGHT;

	
	if (startX >= numCol || startY >= numRow|| endX>=numCol|| endY>=numRow)
		return objects;
	objects.clear();
	for (int i = startX; i <= endX; i++)
	{
		for (int j = startY; j <= endY; j++)
		{
			if (cells[i][j].GetObjects().size() > 0)
			{
				for (int k = 0; k < cells[i][j].GetObjects().size(); k++)
				{
					objects.push_back(cells[i][j].GetObjects().at(k));
				}
			}
		}
	}

	return objects;
}

void CGrid::Unload()
{
	for (int i = 0; i < numCol; i++)
	{
		for (int j = 0; j < numRow; j++)
		{
			cells[i][j].Unload();
		}
	}

	objects.clear();
}