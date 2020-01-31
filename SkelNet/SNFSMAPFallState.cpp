#include "SNFSMAPFallState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMAPFallState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->fallAnim);
}

void SNFSMAPFallState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;
	SNTransform* floorTransform = &fsmData->world->worldFloor.transform;
	//autoProxy->SetDirection();

	if (input->leftStickDirection.y > 0)
	{
		autoProxy->transform.SetAcceleration({ autoProxy->transform.GetAcceleration().x, autoProxy->gravity * (autoProxy->fallGravityMult + (autoProxy->fastFallGravityMult * abs(input->leftStickDirection.y))) });
	}
	else
	{
		autoProxy->transform.SetAcceleration({ autoProxy->transform.GetAcceleration().x, autoProxy->gravity * autoProxy->fallGravityMult });
	}

	// movement time integration
	autoProxy->transform.SetPreviousPosition(autoProxy->transform.GetPosition());

	autoProxy->transform.SetVelocity(autoProxy->transform.GetVelocity() + autoProxy->transform.GetAcceleration() * dt);
	autoProxy->transform.SetPosition(autoProxy->transform.GetPosition() + autoProxy->transform.GetVelocity() * dt);

	if (input->attack)
	{
		autoProxy->EnterState(ATTACK_STATE);
		return;
	}

	if (input->leftStickDirection.x != 0)
	{
		autoProxy->transform.SetAcceleration({ autoProxy->transform.GetAcceleration().x ,autoProxy->accelerationSpeed * autoProxy->airControlMult * input->leftStickDirection.x });
	}

	if (autoProxy->IsGrounded())
	{
		// land
		fsmData->autonomousProxy->animator->IncrementOneFrame();
		autoProxy->EnterState(IDLE_STATE);
	}

}

void SNFSMAPFallState::Exit(SNFSMData* fsmData)
{
	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->autonomousProxy->transform.GetPosition(),
		fsmData->world->landingDustAnim, 8 * 0.05f, fsmData->autonomousProxy->transform.GetFacingRight());

	fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->land);
}
