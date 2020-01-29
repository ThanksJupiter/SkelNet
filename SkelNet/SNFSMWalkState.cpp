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

	if (fsmData->world->HasAuthority())
	{
		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = WALK_STATE;

		fsmData->world->server.SendData(&statePacket);
	}
	else
	{
		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = WALK_STATE;

		fsmData->world->client.SendData(&statePacket);
	}

	timer = 0.0f;
}

void SNFSMWalkState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	timer += dt;

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
	}
	else
	{
		// TODO don't leave walk state if no input
		autoProxy->velocity.x = 0.0f;
		autoProxy->acceleration.x = 0.0f;
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
		return;
	}

	if (abs(input->leftStickDirection.x) > .8 && timer < runTimeThreshold)
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

	autoProxy->SendTransformData();
}

void SNFSMWalkState::Exit(SNFSMData* fsmData)
{
	
}
