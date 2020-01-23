#include "SNFSMKnockbackState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNServer.h"
#include "SNDataPacket.h"

void SNFSMKnockbackState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->knockbackAnim);

	if (fsmData->world->isServer)
	{
		fsmData->world->server.statePack.animState = KNOCKBACK_ANIM;
	}
	else
	{
		fsmData->world->client.statePack.animState = KNOCKBACK_ANIM;
	}
}

void SNFSMKnockbackState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	autoProxy->acceleration.y = autoProxy->gravity * autoProxy->gravityMult;

	// movement time integration
	autoProxy->previousPosition = autoProxy->position;

	autoProxy->velocity += autoProxy->acceleration * dt;
	autoProxy->position += autoProxy->velocity * dt;

	if (autoProxy->velocity.y > 0 && autoProxy->position.y > 333)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}
}

void SNFSMKnockbackState::Exit(SNFSMData* fsmData)
{
	
}
