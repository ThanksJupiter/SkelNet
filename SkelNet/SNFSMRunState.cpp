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
		fsmData->autonomousProxy->transform.GetPosition(),
		fsmData->world->dashDustAnim, 8 * 0.05f, fsmData->autonomousProxy->flip);

	autoProxy->transform.SetVelocity({ 270.0f * input->leftStickDirection.x, autoProxy->transform.GetPosition().y});
	timer = 0.0f;
}

void SNFSMRunState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	timer += dt;

	autoProxy->SetDirection();

	if (abs(autoProxy->transform.GetVelocity().x) < autoProxy->maxVelocitySpeed)
	{
		autoProxy->transform.SetAcceleration({ autoProxy->accelerationSpeed * input->leftStickDirection.x,autoProxy->transform.GetAcceleration().y });
	}
	else
	{
		autoProxy->transform.SetAcceleration({0,autoProxy->transform.GetAcceleration().y});
	}

	bool attemptingDirectionChange = 
		(autoProxy->transform.GetVelocity().x > 0 && input->leftStickDirection.x < 0) ||
		(autoProxy->transform.GetVelocity().x < 0 && input->leftStickDirection.x > 0);

	if (attemptingDirectionChange)
	{
		if (timer < dashDanceThreshold)
		{
			fsmData->stateMachine->EnterState(fsmData->availableStates[RUN_STATE]);
			return;
		}
		else
		{
			fsmData->stateMachine->EnterState(fsmData->availableStates[WALK_STATE]);
			return;
		}
	}

	// TODO implement drag to decrease speed when no input
	if (abs(autoProxy->transform.GetVelocity().x) < autoProxy->minRunSpeed)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[WALK_STATE]);
		return;
	}

	if (input->leftStickDirection.x == 0)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[WALK_STATE]);
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
	autoProxy->transform.SetPreviousPosition(autoProxy->transform.GetPosition());

	autoProxy->transform.SetVelocity(autoProxy->transform.GetVelocity() + autoProxy->transform.GetAcceleration() * dt);
	autoProxy->transform.SetPosition(autoProxy->transform.GetPosition() + autoProxy->transform.GetVelocity() * dt);
}

void SNFSMRunState::Exit(SNFSMData* fsmData)
{
	
}
