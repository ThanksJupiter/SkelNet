#include "SNFSMWalkState.h"
#include "SNFSMData.h"
#include "SNEngine.h"
#include <cmath>
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMWalkState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->walkAnim);

	if (fsmData->world->isServer)
	{
		fsmData->world->server.statePack.animState = WALK_ANIM;
	}
	else
	{
		fsmData->world->client.statePack.animState = WALK_ANIM;
	}
}

void SNFSMWalkState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	autoProxy->SetDirection();

	if (input->leftStickDirection.x != 0)
	{
		if (abs(autoProxy->velocity.x) < autoProxy->minVelocitySpeed)
		{
			autoProxy->velocity.x = autoProxy->minVelocitySpeed * input->leftStickDirection.x;
		}

		if (abs(autoProxy->velocity.x) < autoProxy->minRunSpeed)
		{
			autoProxy->acceleration.x = autoProxy->accelerationSpeed * input->leftStickDirection.x;
		}
		else
		{
			fsmData->stateMachine->EnterState(fsmData->availableStates[RUN_STATE]);
			return;
		}
	}
	else
	{
		// TODO don't leave walk state if no input
		autoProxy->velocity.x = 0.0f;
		autoProxy->acceleration.x = 0.0f;
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
		return;
	}

	if (abs(input->leftStickDirection.x) > .8)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[RUN_STATE]);
		return;
	}

	if (input->jump)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[JUMP_STATE]);
		return;
	}

	if (input->attack)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[ATTACK_STATE]);
		return;
	}

	// movement time integration
	autoProxy->previousPosition = autoProxy->position;

	autoProxy->velocity += autoProxy->acceleration * dt;
	autoProxy->position += autoProxy->velocity * dt;
}

void SNFSMWalkState::Exit(SNFSMData* fsmData)
{
	
}
