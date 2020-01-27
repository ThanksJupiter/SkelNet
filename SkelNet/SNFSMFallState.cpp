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
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	autoProxy->SetDirection();
	autoProxy->acceleration.y = autoProxy->gravity * autoProxy->gravityMult;

	autoProxy->previousPosition = autoProxy->position;

	autoProxy->velocity += autoProxy->acceleration * dt;
	autoProxy->position += autoProxy->velocity * dt;

	if (input->leftStickDirection.x != 0)
	{
		autoProxy->acceleration.x = autoProxy->accelerationSpeed * autoProxy->airControlMult * input->leftStickDirection.x;
	}

	// movement time integration
	autoProxy->previousPosition = autoProxy->position;

	return;

	if ((autoProxy->velocity.y > 0 && autoProxy->position.y > 333) && (autoProxy->position.x > 170 || autoProxy->position.x < 935))
	{
		// land
		fsmData->autonomousProxy->animator->IncrementOneFrame();
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}
}

void SNFSMFallState::Exit(SNFSMData* fsmData)
{

}
