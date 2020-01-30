#include "SNFSMKnockedDownState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMKnockedDownState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->knockedDownAnim);
}

void SNFSMKnockedDownState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	if (input->leftStickDirection.x != 0)
	{
		autoProxy->EnterState(IDLE_STATE);
	}
	if (input->jump)
	{
		autoProxy->EnterState(JUMP_STATE);
	}
}

void SNFSMKnockedDownState::Exit(SNFSMData* fsmData)
{
	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->autonomousProxy->transform.GetPosition(),
		fsmData->world->landingDustAnim, 8 * 0.05f, fsmData->autonomousProxy->flip);
}
