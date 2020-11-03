#include "PowerHub.h"

CPowerHub::CPowerHub()
{
	
}

void CPowerHub::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}

void CPowerHub::Render()
{
	animation_set->at(POWERHUB_ANI_LEVEL)->isHubObject = true;
	animation_set->at(POWERHUB_ANI_TEXT)->isHubObject = true;

	animation_set->at(POWERHUB_ANI_LEVEL)->Render(x, y);
	animation_set->at(POWERHUB_ANI_TEXT)->Render(x, y+40);
}
void CPowerHub::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
}