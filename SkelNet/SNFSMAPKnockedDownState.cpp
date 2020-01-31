#include "SNFSMAPKnockedDownState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"
#include "SNTransform.h"

void SNFSMAPKnockedDownState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->knockedDownAnim);
}

void SNFSMAPKnockedDownState::Update(float dt, SNFSMData* fsmData)
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

void SNFSMAPKnockedDownState::Exit(SNFSMData* fsmData)
{
	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->autonomousProxy->transform.GetPosition(),
		fsmData->world->landingDustAnim, 8 * 0.05f, fsmData->autonomousProxy->transform.GetFacingRight());
}
