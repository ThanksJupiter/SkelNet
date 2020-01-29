#include "SNFSMAttackState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMAttackState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->apAttackAnim);
	//fsmData->autonomousProxy->velocity.x = 0;

	hasMissSoundPlayed = false;
	hasStartSoundPlayed = false;

	timer = 0.0f;
	fsmData->autonomousProxy->Attack();
	hit = false;
}

void SNFSMAttackState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	if (input->leftStickDirection.y > 0)
	{
		autoProxy->acceleration.y = autoProxy->gravity * (autoProxy->fallGravityMult + (autoProxy->fastFallGravityMult * abs(input->leftStickDirection.y)));
	}
	else
	{
		autoProxy->acceleration.y = autoProxy->gravity * autoProxy->fallGravityMult;
	}

	timer += dt;

	if (timer >= startupSoundDelay && !hasStartSoundPlayed)
	{
		fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->whip_start);
		hasStartSoundPlayed = true;
	}

	if (timer >= missSoundDelay && !hasMissSoundPlayed)
	{
		fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->whip_miss);
		hasMissSoundPlayed = true;
	}

	if (input->leftStickDirection.x != 0)
	{
		autoProxy->acceleration.x = autoProxy->accelerationSpeed * autoProxy->airControlMult * input->leftStickDirection.x;
	}

	if (timer >= checkAttackDuration && !hit)
	{
		hit = true;
		
		if (fsmData->world->isServer)
		{
			APDoAttack(fsmData->world);
		}
		else
		{
			SPDoAttack(fsmData->world);
		}
	}

	autoProxy->previousPosition = autoProxy->position;

	autoProxy->velocity += autoProxy->acceleration * dt;
	autoProxy->position += autoProxy->velocity * dt;

	if ((autoProxy->velocity.y > 0 && autoProxy->position.y > 333) && (autoProxy->position.x > 170 && autoProxy->position.x < 935))
	{
		// land
		autoProxy->velocity.x = 0;
	}

	if (timer >= attackDuration)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}
}

void SNFSMAttackState::Exit(SNFSMData* fsmData)
{
	
}
