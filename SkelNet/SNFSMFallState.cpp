#include "SNFSMFallState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"

void SNFSMFallState::Enter(SNFSMData* fsmData)
{
	
}

void SNFSMFallState::Update(float dt, SNFSMData* fsmData)
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
		fsmData->autonomousProxy->animator->rotation = 0;
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}
}

void SNFSMFallState::Exit(SNFSMData* fsmData)
{
	
}
