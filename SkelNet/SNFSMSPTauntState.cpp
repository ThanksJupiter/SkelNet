#include "SNFSMSPTauntState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"

void SNFSMSPTauntState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->teabagAnim);
}

void SNFSMSPTauntState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPTauntState::Exit(SNFSMData* fsmData)
{

}
