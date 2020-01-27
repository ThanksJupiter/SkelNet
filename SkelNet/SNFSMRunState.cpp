#include "SNFSMRunState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNParticleSystem.h"

void SNFSMRunState::Enter(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->runAnim);

	if (fsmData->world->isServer)
	{
		fsmData->world->server.statePack.animState = RUN_ANIM;
	}
	else
	{
		fsmData->world->client.statePack.animState = RUN_ANIM;
	}

	autoProxy->SetDirection();

	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->autonomousProxy->position,
		fsmData->world->dashDustAnim, 8 * 0.05f, fsmData->autonomousProxy->flip);

	autoProxy->velocity.x = 270.0f * input->leftStickDirection.x;
}

void SNFSMRunState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	autoProxy->SetDirection();

	if (abs(autoProxy->velocity.x) < autoProxy->maxVelocitySpeed)
	{
		autoProxy->acceleration.x = autoProxy->accelerationSpeed * input->leftStickDirection.x;
	}
	else
	{
		autoProxy->acceleration.x = 0;
	}

	bool attemptingDirectionChange = 
		(autoProxy->velocity.x > 0 && input->leftStickDirection.x < 0) ||
		(autoProxy->velocity.x < 0 && input->leftStickDirection.x > 0);

	if (attemptingDirectionChange)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
		return;
	}

	// TODO implement drag to decrease speed when no input
	if (abs(autoProxy->velocity.x) < autoProxy->minRunSpeed)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[WALK_STATE]);
		return;
	}

	if (input->leftStickDirection.x == 0)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
		return;
	}

	if (input->attack)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[ATTACK_STATE]);
		return;
	}

	if (input->jump)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[JUMP_STATE]);
		return;
	}

	// movement time integration
	autoProxy->previousPosition = autoProxy->position;

	autoProxy->velocity += autoProxy->acceleration * dt;
	autoProxy->position += autoProxy->velocity * dt;
}

void SNFSMRunState::Exit(SNFSMData* fsmData)
{
	
}
