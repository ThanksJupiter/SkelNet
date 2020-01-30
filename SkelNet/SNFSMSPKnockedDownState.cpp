#include "SNFSMSPKnockedDownState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"

void SNFSMSPKnockedDownState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->knockedDownAnim);
}

void SNFSMSPKnockedDownState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPKnockedDownState::Exit(SNFSMData* fsmData)
{

}
