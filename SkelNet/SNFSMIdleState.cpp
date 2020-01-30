#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMIdleState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->idleAnim);
	fsmData->autonomousProxy->transform.SetVelocity({ 0, fsmData->autonomousProxy->transform.GetVelocity().y });

	if (fsmData->world->isServer)
	{
		fsmData->world->server.statePack.animState = IDLE_ANIM;
	}
	else
	{
		fsmData->world->client.statePack.animState = IDLE_ANIM;
	}
}

void SNFSMIdleState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	//autoProxy->SetDirection();

	if (autoProxy->flip && input->leftStickDirection.x > 0)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[TURNAROUND_STATE]);
		return;
	}
	else if (!autoProxy->flip && input->leftStickDirection.x < 0)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[TURNAROUND_STATE]);
		return;
	}

	if (input->leftStickDirection.x != 0)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[WALK_STATE]);
		return;
	}

	if (input->jump)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[JUMP_STATE]);
		return;
	}

	if (input->attack)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[ATTACK_STATE]);
		return;
	}

	if (input->taunt)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[TAUNT_STATE]);
		return;
	}
}

void SNFSMIdleState::Exit(SNFSMData* fsmData)
{

}
