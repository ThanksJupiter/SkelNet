#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMAPIdleState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->idleAnim);
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

	if (autoProxy->flip && input->leftStickDirection.x > 0)
	{
		autoProxy->EnterState(TURNAROUND_STATE);
		return;
	}
	else if (!autoProxy->flip && input->leftStickDirection.x < 0)
	{
		autoProxy->EnterState(TURNAROUND_STATE);
		return;
	}

	if (input->leftStickDirection.x != 0)
	{
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
}

void SNFSMAPIdleState::Exit(SNFSMData* fsmData)
{

}
