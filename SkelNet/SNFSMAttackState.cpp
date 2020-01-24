#include "SNFSMAttackState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMAttackState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->apAttackAnim);
	fsmData->autonomousProxy->velocity.x = 0;
	timer = 0.0f;
	fsmData->autonomousProxy->Attack();
	hit = false;
}

void SNFSMAttackState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= checkAttackDuration && !hit)
	{
		hit = true;
		if (fsmData->world->isServer)
		{
			APDoAttack(fsmData->world);
		}
		else
		{
			SPDoAttack(fsmData->world);
		}
	}

	if (timer >= attackDuration)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}
}

void SNFSMAttackState::Exit(SNFSMData* fsmData)
{
	
}
