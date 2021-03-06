#include "SNFSMAPRunState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNParticleSystem.h"

void SNFSMAPRunState::Enter(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->runAnim);

	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->autonomousProxy->transform.GetPosition(),
		fsmData->world->dashDustAnim, 8 * 0.05f, fsmData->autonomousProxy->transform.GetFacingRight());

	autoProxy->transform.SetVelocity({ 270.0f * input->leftStickDirection.x, autoProxy->transform.GetPosition().y});
	timer = 0.0f;
}

void SNFSMAPRunState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	timer += dt;

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
		autoProxy->EnterState(TURNAROUND_STATE);
		return;
	}

	// TODO implement drag to decrease speed when no input
	if (abs(autoProxy->transform.GetVelocity().x) < autoProxy->minRunSpeed)
	{
		autoProxy->EnterState(WALK_STATE);
		return;
	}

	/*if (input->leftStickDirection.x == 0)
	{
		autoProxy->EnterState(IDLE_STATE);
		return;
	}*/

	if (input->attack)
	{
		autoProxy->EnterState(ATTACK_STATE);
		return;
	}

	if (input->jump)
	{
		autoProxy->EnterState(JUMPSQUAT_STATE);
		return;
	}

	if (!autoProxy->IsGrounded())
	{
		autoProxy->EnterState(FALL_STATE);
		return;
	}

	autoProxy->ForcesTimeIntegration(dt);
}

void SNFSMAPRunState::Exit(SNFSMData* fsmData)
{

}
