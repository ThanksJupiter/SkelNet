#include "SNFSMSPDashAttackState.h"
#include "SNWorld.h"
#include "SNSimulatedProxy.h"
#include "SNFSMData.h"
#include "SNAnimator.h"

void SNFSMSPDashAttackState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->dashTackleAnim);
}

void SNFSMSPDashAttackState::Update(float dt, SNFSMData* fsmData)
{
	
}

void SNFSMSPDashAttackState::Exit(SNFSMData* fsmData)
{
	
}
