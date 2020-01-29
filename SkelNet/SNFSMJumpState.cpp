#include "SNFSMJumpState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNParticleSystem.h"
#include "SNEngine.h"

void SNFSMJumpState::Enter(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	autoProxy->animator->doManualAnimationCycling = true;
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->jumpAnim);

	autoProxy->velocity.x = 100.0f * fsmData->input->leftStickDirection.x;
	autoProxy->world->audioManager->PlayChunkOnce(autoProxy->world->audioManager->jump);

	timer = 0.0f;
	jumped = false;
}

void SNFSMJumpState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;
	
	autoProxy->SetDirection();
	autoProxy->acceleration.y = autoProxy->gravity * (input->jumpHeld ? autoProxy->gravityMult : autoProxy->fallGravityMult);

	timer += dt;

	if (timer > jumpDelay && !jumped)
	{
		jumped = true;
		DoJump(autoProxy);
		// movement time integration
		autoProxy->previousPosition = autoProxy->position;

		autoProxy->velocity += autoProxy->acceleration * dt;
		autoProxy->position += autoProxy->velocity * dt;
		return;
	}
	else
	{
		if (!jumped)
		{
			return;
		}
	}

	if (input->attack)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[ATTACK_STATE]);
		return;
	}

	if (input->leftStickDirection.x != 0)
	{
		autoProxy->acceleration.x = autoProxy->accelerationSpeed * autoProxy->airControlMult * input->leftStickDirection.x;
	}

	// movement time integration
	autoProxy->previousPosition = autoProxy->position;

	autoProxy->velocity += autoProxy->acceleration * dt;
	autoProxy->position += autoProxy->velocity * dt;

	if (autoProxy->velocity.y > 0)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[FALL_STATE]);
	}

	if (autoProxy->velocity.y > 0 && autoProxy->position.y > 333)
	{
		// land
		LeaveLandingFrame(fsmData);
	}
}

void SNFSMJumpState::Exit(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	autoProxy->animator->doManualAnimationCycling = false;
}

void SNFSMJumpState::DoJump(SNAutonomousProxy* autoProxy)
{
	autoProxy->acceleration.x = 0.0f;
	autoProxy->velocity.y = -200.0f;
	//autoProxy->position.y -= 5.0f;
	timer = 0.0f;
	autoProxy->animator->IncrementOneFrame();
}

void SNFSMJumpState::LeaveLandingFrame(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->IncrementOneFrame();
	fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
}
