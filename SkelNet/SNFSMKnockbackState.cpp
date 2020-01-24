#include "SNFSMKnockbackState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNServer.h"
#include "SNDataPacket.h"
#include "Vector.h"
#include <cmath>

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
	angle = 0;
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

	if (fsmData->world->autonomousProxy.position.x > fsmData->world->simulatedProxy.position.x)
	{
		angle += 10 * dt;//std::acos(Dot({ -1, 0 }, Normalize(autoProxy->velocity)));
	}
	else
	{
		angle -= 10 * dt;
	}
	fsmData->autonomousProxy->animator->rotation = angle * (180.f / 3.14159f);

	if (autoProxy->velocity.y > 0 && autoProxy->position.y > 333)
	{
		fsmData->autonomousProxy->animator->rotation = 0;
		fsmData->stateMachine->EnterState(fsmData->availableStates[IDLE_STATE]);
	}
}

void SNFSMKnockbackState::Exit(SNFSMData* fsmData)
{
	
}
