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

	timer += dt;

	if (timer >= dustDelay)
	{
		fsmData->world->particleSystem->StartParticleEffect(
			fsmData->autonomousProxy->transform.GetPosition(),
			fsmData->world->dustCloud01Anim, fsmData->world->dustCloud01Anim->duration, fsmData->autonomousProxy->flip);
		timer = 0.0f;
	}

	autoProxy->transform.SetAcceleration({ autoProxy->transform.GetAcceleration().x , autoProxy->gravity * autoProxy->gravityMult });

	// movement time integration
	autoProxy->transform.SetPreviousPosition(autoProxy->transform.GetPosition());

	autoProxy->transform.SetVelocity(autoProxy->transform.GetVelocity() + autoProxy->transform.GetAcceleration() * dt);
	autoProxy->transform.SetPosition(autoProxy->transform.GetPosition() + autoProxy->transform.GetVelocity() * dt);

	if (autoProxy->transform.GetVelocity().y > 0 && autoProxy->transform.GetPosition().y > 333)
	{
		fsmData->autonomousProxy->animator->rotation = 0;
		fsmData->stateMachine->EnterState(fsmData->availableStates[KNOCKDOWN_STATE]);
	}
}

void SNFSMKnockbackState::Exit(SNFSMData* fsmData)
{
	
}
