#include "SNFSMSPAttackState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"

void SNFSMSPAttackState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->spAttackAnim);
}

void SNFSMSPAttackState::Update(float dt, SNFSMData* fsmData)
{
	
}

void SNFSMSPAttackState::Exit(SNFSMData* fsmData)
{
	
}
