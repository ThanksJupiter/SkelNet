#include "SNFSMSPWalkState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"



void SNFSMSPWalkState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->walkAnim);
}

void SNFSMSPWalkState::Update(float dt, SNFSMData* fsmData)
{
	
}

void SNFSMSPWalkState::Exit(SNFSMData* fsmData)
{
	
}
