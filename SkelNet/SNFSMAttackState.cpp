#include "SNFSMAttackState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMAttackState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->attackAnim);
	fsmData->autonomousProxy->velocity.x = 0;
	timer = 0.0f;
}

void SNFSMAttackState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= attackDuration)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}
}

void SNFSMAttackState::Exit(SNFSMData* fsmData)
{
	
}
