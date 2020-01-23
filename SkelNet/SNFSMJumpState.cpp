#include "SNFSMJumpState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMJumpState::Enter(SNFSMData* fsmData)
{
	//fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->attackAnim);

	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	autoProxy->acceleration.x = 0.0f;
	autoProxy->velocity.y = -200.0f;
}

void SNFSMJumpState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;
	autoProxy->SetDirection();
	
	autoProxy->acceleration.y = autoProxy->gravity * autoProxy->gravityMult;

	if (input->leftStickDirection.x != 0)
	{
		autoProxy->acceleration.x = autoProxy->accelerationSpeed * autoProxy->airControlMult * input->leftStickDirection.x;
	}

	// movement time integration
	autoProxy->previousPosition = autoProxy->position;

	autoProxy->velocity += autoProxy->acceleration * dt;
	autoProxy->position += autoProxy->velocity * dt;

	if (autoProxy->velocity.y > 0 && autoProxy->position.y > 333)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}
}

void SNFSMJumpState::Exit(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	autoProxy->position.y = 333;
	autoProxy->velocity.y = 0;
}
