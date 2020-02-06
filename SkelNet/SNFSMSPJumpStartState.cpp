#include "SNFSMSPJumpStartState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"


void SNFSMSPJumpStartState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->jumpAnim);
	fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->jump);
}

void SNFSMSPJumpStartState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPJumpStartState::Exit(SNFSMData* fsmData)
{

}
