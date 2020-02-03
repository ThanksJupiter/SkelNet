#include "SNFSMAPAttackState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMAPAttackState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->apAttackAnim);

	hasMissSoundPlayed = false;
	hasStartSoundPlayed = false;

	timer = 0.0f;
	fsmData->autonomousProxy->SendEnterAttackState();
	hit = false;
}

void SNFSMAPAttackState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;
	timer += dt;

	if (input->leftStickDirection.y > 0)
	{
		autoProxy->transform.SetAcceleration({ autoProxy->transform.GetAcceleration().x, autoProxy->gravity * (autoProxy->fallGravityMult + (autoProxy->fastFallGravityMult * abs(input->leftStickDirection.y))) });
	}
	else
	{
		autoProxy->transform.SetAcceleration({ autoProxy->transform.GetAcceleration().x, autoProxy->gravity * autoProxy->fallGravityMult });
	}

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

		if (fsmData->world->HasAuthority())
		{
			autoProxy->DoAttack();
		}
		else
		{
			//SPDoAttack(fsmData->world);
		}
	}

	autoProxy->ForcesTimeIntegration(dt);

	if (autoProxy->IsGrounded())
	{
		autoProxy->transform.SetVelocity({ 0, autoProxy->transform.GetVelocity().y });
	}

	/*if ((autoProxy->transform.GetVelocity().y > 0 && autoProxy->transform.GetPosition().y > 333) && (autoProxy->transform.GetPosition().x > 170 && autoProxy->transform.GetPosition().x < 935))
	{
		// land
	}*/

	if (timer >= attackDuration)
	{
		autoProxy->EnterState(IDLE_STATE);
	}
}

void SNFSMAPAttackState::Exit(SNFSMData* fsmData)
{

}
