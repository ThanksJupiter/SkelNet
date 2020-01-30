#include "SNFSMSPKnockbackState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"

void SNFSMSPKnockbackState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->knockbackAnim);
}

void SNFSMSPKnockbackState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPKnockbackState::Exit(SNFSMData* fsmData)
{

}
