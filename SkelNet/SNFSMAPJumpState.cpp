#include "SNFSMAPJumpState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNParticleSystem.h"
#include "SNEngine.h"

void SNFSMAPJumpState::Enter(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	autoProxy->animator->doManualAnimationCycling = true;
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->jumpAnim);

	autoProxy->transform.SetVelocity({ 100.0f * fsmData->input->leftStickDirection.x, autoProxy->transform.GetVelocity().y });
	autoProxy->world->audioManager->PlayChunkOnce(autoProxy->world->audioManager->jump);

	timer = 0.0f;
	jumped = false;

	
}

void SNFSMAPJumpState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	//autoProxy->SetDirection();
	autoProxy->transform.SetAcceleration({ autoProxy->transform.GetAcceleration().x, autoProxy->gravity * (input->jumpHeld ? autoProxy->gravityMult : autoProxy->fallGravityMult) });
	timer += dt;

	if (timer > jumpDelay && !jumped)
	{
		jumped = true;
		DoJump(autoProxy);

		// movement time integration
		autoProxy->transform.SetPreviousPosition(autoProxy->transform.GetPosition());

		autoProxy->transform.SetVelocity(autoProxy->transform.GetVelocity() + autoProxy->transform.GetAcceleration() * dt);
		autoProxy->transform.SetPosition(autoProxy->transform.GetPosition() + autoProxy->transform.GetVelocity() * dt);
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
		autoProxy->EnterState(ATTACK_STATE);
		return;
	}

	if (input->leftStickDirection.x != 0)
	{
		autoProxy->transform.SetAcceleration({ autoProxy->accelerationSpeed * autoProxy->airControlMult * input->leftStickDirection.x, autoProxy->transform.GetAcceleration().y });
	}

	// movement time integration
	autoProxy->transform.SetPreviousPosition(autoProxy->transform.GetPosition());

	autoProxy->transform.SetVelocity(autoProxy->transform.GetVelocity() + autoProxy->transform.GetAcceleration() * dt);
	autoProxy->transform.SetPosition(autoProxy->transform.GetPosition() + autoProxy->transform.GetVelocity() * dt);

	if (autoProxy->transform.GetVelocity().y > 0)
	{
		autoProxy->EnterState(FALL_STATE);
	}

	if (autoProxy->IsGrounded())
	{
		// land
		LeaveLandingFrame(fsmData);
	}
}

void SNFSMAPJumpState::Exit(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	autoProxy->animator->doManualAnimationCycling = false;
}

void SNFSMAPJumpState::DoJump(SNAutonomousProxy* autoProxy)
{
	autoProxy->transform.SetAcceleration({ 0,autoProxy->transform.GetPosition().y });
	autoProxy->transform.SetVelocity({ autoProxy->transform.GetVelocity().x, -200.0f });
	//autoProxy->position.y -= 5.0f;
	timer = 0.0f;
	autoProxy->animator->IncrementOneFrame();
}

void SNFSMAPJumpState::LeaveLandingFrame(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	fsmData->autonomousProxy->animator->IncrementOneFrame();
	autoProxy->EnterState(IDLE_STATE);
}
