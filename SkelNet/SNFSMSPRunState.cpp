#include "SNFSMSPRunState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"

void SNFSMSPRunState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->runAnim);
}

void SNFSMSPRunState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPRunState::Exit(SNFSMData* fsmData)
{

}
