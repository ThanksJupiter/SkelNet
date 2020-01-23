#include "SNFSMKnockbackState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMKnockbackState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->runAnim);
}

void SNFSMKnockbackState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	autoProxy->acceleration.y = autoProxy->gravity * autoProxy->gravityMult;

	// movement time integration
	autoProxy->previousPosition = autoProxy->position;

	autoProxy->velocity += autoProxy->acceleration * dt;
	autoProxy->position += autoProxy->velocity * dt;

	if (autoProxy->velocity.y > 0 && autoProxy->position.y > 333)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}
}

void SNFSMKnockbackState::Exit(SNFSMData* fsmData)
{
	
}
