#include "SNFSMSPJumpSquatState.h"
#include "SNFSMData.h"
#include "SNAnimator.h"
#include "SNAnimation.h"
#include "SNWorld.h"

void SNFSMSPJumpSquatState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->jumpSquatAnim);
}

void SNFSMSPJumpSquatState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPJumpSquatState::Exit(SNFSMData* fsmData)
{

}
