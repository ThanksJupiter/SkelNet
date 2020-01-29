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
		if (abs(autoProxy->transform.GetVelocity().x) < autoProxy->minVelocitySpeed)
		{
			autoProxy->transform.SetVelocity({ autoProxy->minVelocitySpeed * input->leftStickDirection.x,autoProxy->transform.GetVelocity().y });
		}

		if (abs(autoProxy->transform.GetVelocity().x) < autoProxy->minRunSpeed)
		{
			autoProxy->transform.SetAcceleration({ autoProxy->accelerationSpeed * input->leftStickDirection.x, autoProxy->transform.GetAcceleration().y });
		}
	}
	else
	{
		// TODO don't leave walk state if no input
		autoProxy->transform.SetVelocity({ 0.0f, autoProxy->transform.GetVelocity().y});
		autoProxy->transform.SetAcceleration({ 0.0f, autoProxy->transform.GetAcceleration().y });

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
	autoProxy->transform.SetPreviousPosition(autoProxy->transform.GetPosition());

	autoProxy->transform.SetVelocity(autoProxy->transform.GetVelocity() + autoProxy->transform.GetAcceleration() * dt);
	autoProxy->transform.SetPosition(autoProxy->transform.GetPosition() + autoProxy->transform.GetVelocity() * dt);
}

void SNFSMWalkState::Exit(SNFSMData* fsmData)
{
	
}
