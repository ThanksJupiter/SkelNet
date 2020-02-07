#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNTransform.h"
#include "SNParticleSystem.h"

void SNFSMAPIdleState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->idleAnim);

	fsmData->autonomousProxy->transform.SetAcceleration({ 0, 0 });
	fsmData->autonomousProxy->transform.SetVelocity({ 0, 0 });
}

void SNFSMAPIdleState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	if (!autoProxy->IsGrounded())
	{
		autoProxy->EnterState(FALL_STATE);
	}

	if (input->leftStickDirection.x != 0)
	{
		autoProxy->SetDirection();
		autoProxy->EnterState(WALK_STATE);
		return;
	}

	if (input->jump)
	{
		autoProxy->EnterState(JUMPSQUAT_STATE);
		return;
	}

	if (input->attack)
	{
		autoProxy->EnterState(ATTACK_STATE);
		return;
	}

	if (input->downTaunt)
	{
		autoProxy->EnterState(TEABAG_STATE);
		return;
	}

	if (input->upTaunt)
	{
		autoProxy->EnterState(SMOKE_STATE);
		return;
	}

	if (input->rightTaunt)
	{
		autoProxy->EnterState(DOOT_STATE);
		return;
	}

	autoProxy->ForcesTimeIntegration(dt);

	/*if (autoProxy->IsGrounded())
	{
		SNTransform* transform = &autoProxy->transform;
		SNTransform floorTransform = fsmData->world->worldFloor.transform;
		transform->SetPosition({ transform->GetPosition().x, floorTransform.GetPosition().y });
		transform->SetVelocity({ transform->GetVelocity().x, 0 });
	}*/
}

void SNFSMAPIdleState::Exit(SNFSMData* fsmData)
{

}
