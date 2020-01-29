#include "SNFSMKnockbackState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNServer.h"
#include "SNDataPacket.h"
#include "Vector.h"
#include <cmath>
#include "SNParticleSystem.h"
#include "SNAnimation.h"

void SNFSMKnockbackState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->knockbackAnim);

	if (fsmData->world->HasAuthority())
	{
		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = KNOCKBACK_STATE;

		fsmData->world->server.SendData(&statePacket);
	}
}

void SNFSMKnockbackState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	timer += dt;

	if (timer >= dustDelay)
	{
		fsmData->world->particleSystem->StartParticleEffect(
			fsmData->autonomousProxy->position,
			fsmData->world->dustCloud01Anim, fsmData->world->dustCloud01Anim->duration, fsmData->autonomousProxy->flip);
		timer = 0.0f;
	}

	autoProxy->acceleration.y = autoProxy->gravity * autoProxy->gravityMult;

	// movement time integration
	autoProxy->previousPosition = autoProxy->position;

	autoProxy->velocity += autoProxy->acceleration * dt;
	autoProxy->position += autoProxy->velocity * dt;

	if (autoProxy->velocity.y > 0 && autoProxy->position.y > 333)
	{
		fsmData->autonomousProxy->animator->rotation = 0;
		fsmData->stateMachine->EnterState(fsmData->availableStates[KNOCKDOWN_STATE]);
	}
}

void SNFSMKnockbackState::Exit(SNFSMData* fsmData)
{
	
}
