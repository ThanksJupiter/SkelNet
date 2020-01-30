#include "SNFSMSPIdleState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"


void SNFSMSPIdleState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->idleAnim);
}

void SNFSMSPIdleState::Update(float dt, SNFSMData* fsmData)
{
	
}

void SNFSMSPIdleState::Exit(SNFSMData* fsmData)
{
	
}
