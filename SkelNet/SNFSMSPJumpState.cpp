#include "SNFSMSPJumpState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"


void SNFSMSPJumpState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->jumpAnim);
}

void SNFSMSPJumpState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPJumpState::Exit(SNFSMData* fsmData)
{

}
