#include "SNFSMSPTurnAroundState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"


void SNFSMSPTurnAroundState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->turnAroundAnim);
}

void SNFSMSPTurnAroundState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPTurnAroundState::Exit(SNFSMData* fsmData)
{
	SNSimulatedProxy* simProxy = fsmData->simulatedProxy;
	simProxy->transform.SetFacingRight(!simProxy->transform.GetFacingRight());
}
