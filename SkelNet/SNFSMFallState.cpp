#include "SNFSMFallState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMFallState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->fallAnim);
}

void SNFSMFallState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	autoProxy->SetDirection();

	if (input->leftStickDirection.y > 0)
	{
		autoProxy->acceleration.y = autoProxy->gravity * (autoProxy->fallGravityMult + (autoProxy->fastFallGravityMult * abs(input->leftStickDirection.y)));
	}
	else
	{
		autoProxy->acceleration.y = autoProxy->gravity * autoProxy->fallGravityMult;
	}

	// movement time integration
	autoProxy->previousPosition = autoProxy->position;

	autoProxy->velocity += autoProxy->acceleration * dt;
	autoProxy->position += autoProxy->velocity * dt;

	if (input->attack)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[ATTACK_STATE]);
		return;
	}

	if (input->leftStickDirection.x != 0)
	{
		autoProxy->acceleration.x = autoProxy->accelerationSpeed * autoProxy->airControlMult * input->leftStickDirection.x;
	}

	if ((autoProxy->velocity.y > 0 && autoProxy->position.y > 333) && (autoProxy->position.x > 170 && autoProxy->position.x < 935))
	{
		// land
		fsmData->autonomousProxy->animator->IncrementOneFrame();
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}

	autoProxy->SendTransformData();
}

void SNFSMFallState::Exit(SNFSMData* fsmData)
{
	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->autonomousProxy->position,
		fsmData->world->landingDustAnim, 8 * 0.05f, fsmData->autonomousProxy->flip);

	fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->land);
}
