#include "SNFSMSPDootState.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNWorld.h"

void SNFSMSPDootState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->dootAnim);
}

void SNFSMSPDootState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPDootState::Exit(SNFSMData* fsmData)
{

}
