#include "SNFSMSPDootState.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNWorld.h"

void SNFSMSPDootState::Enter(SNFSMData* fsmData)
{
	if (fsmData->stateMachine->previousStateIndex == DOOT_STATE)
	{
		fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->dootAnim, 9);
	}
	else
	{
		fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->dootAnim);
	}
}

void SNFSMSPDootState::Update(float dt, SNFSMData* fsmData)
{
	
}

void SNFSMSPDootState::Exit(SNFSMData* fsmData)
{
	
}
