#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNTransform.h"

void SNFSMAPIdleState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->idleAnim);
	//fsmData->autonomousProxy->transform.SetVelocity({ 0, 0 });


}

void SNFSMAPIdleState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	//autoProxy->SetDirection();

	if (!autoProxy->IsGrounded())
	{
		autoProxy->EnterState(FALL_STATE);
	}

	/*if (autoProxy->transform.GetFacingRight() && input->leftStickDirection.x > 0)
	{
		autoProxy->EnterState(TURNAROUND_STATE);
		return;
	}
	else if (!autoProxy->transform.GetFacingRight() && input->leftStickDirection.x < 0)
	{
		autoProxy->EnterState(TURNAROUND_STATE);
		return;
	}*/

	if (input->leftStickDirection.x != 0)
	{
		autoProxy->SetDirection();
		autoProxy->EnterState(WALK_STATE);
		return;
	}

	if (input->jump)
	{
		autoProxy->EnterState(JUMP_STATE);
		return;
	}

	if (input->attack)
	{
		autoProxy->EnterState(ATTACK_STATE);
		return;
	}

	if (input->taunt)
	{
		autoProxy->EnterState(TAUNT_STATE);
		return;
	}

	autoProxy->ForcesTimeIntegration(dt);
}

void SNFSMAPIdleState::Exit(SNFSMData* fsmData)
{

}
