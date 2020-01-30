#include "SNFSMSPFallState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"

void SNFSMSPFallState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->fallAnim);
}

void SNFSMSPFallState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPFallState::Exit(SNFSMData* fsmData)
{

}
