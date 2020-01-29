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
		autoProxy->transform.SetAcceleration({ autoProxy->transform.GetAcceleration().x, autoProxy->gravity * (autoProxy->fallGravityMult + (autoProxy->fastFallGravityMult * abs(input->leftStickDirection.y))) });
	}
	else
	{
		autoProxy->transform.SetAcceleration({ autoProxy->transform.GetAcceleration().x, autoProxy->gravity * autoProxy->fallGravityMult });
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
		autoProxy->transform.SetAcceleration({ autoProxy->accelerationSpeed * autoProxy->airControlMult * input->leftStickDirection.x, autoProxy->transform.GetAcceleration().y });
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

	autoProxy->transform.SetPreviousPosition(autoProxy->transform.GetPosition());

	autoProxy->transform.SetVelocity(autoProxy->transform.GetVelocity() + autoProxy->transform.GetAcceleration() * dt);
	autoProxy->transform.SetPosition(autoProxy->transform.GetPosition() + autoProxy->transform.GetVelocity() * dt);

	if ((autoProxy->transform.GetVelocity().y > 0 && autoProxy->transform.GetPosition().y > 333) && (autoProxy->transform.GetPosition().x > 170 && autoProxy->transform.GetPosition().x < 935))
	{
		// land
		autoProxy->transform.SetVelocity({ 0, autoProxy->transform.GetVelocity().y });
	}

	if (timer >= attackDuration)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}
}

void SNFSMAttackState::Exit(SNFSMData* fsmData)
{

}
