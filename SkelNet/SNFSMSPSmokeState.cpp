#include "SNFSMSPSmokeState.h"
#include "SNFSMData.h"
#include "SNAnimator.h"
#include "SNWorld.h"


void SNFSMSPSmokeState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->smokeAnim);
}

void SNFSMSPSmokeState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPSmokeState::Exit(SNFSMData* fsmData)
{

}
