#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMIdleState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->idleAnim);
	fsmData->autonomousProxy->velocity.x = 0;
}

void SNFSMIdleState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	autoProxy->SetDirection();

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
}

void SNFSMIdleState::Exit(SNFSMData* fsmData)
{
	
}
