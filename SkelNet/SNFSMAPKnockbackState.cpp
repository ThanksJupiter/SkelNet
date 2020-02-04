#include "SNFSMAPKnockbackState.h"
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
#include <string>
#include "SNEngine.h"

void SNFSMAPKnockbackState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->transform.SetVelocity({0, 0});
	fsmData->autonomousProxy->transform.SetAcceleration({ 0, 0 });
	fsmData->autonomousProxy->TakeDamage();
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->knockbackAnim);

	std::string healthString = std::to_string(fsmData->autonomousProxy->health);
	healthString.push_back('%');
	fsmData->world->autoProxyHealthText->UpdateText(healthString);
	fsmData->world->autoProxyHealthText->SetRelativePosition(
		{ -engGetTextSize(healthString.c_str()).x * 2,
		0 });

	if (fsmData->world->HasAuthority())
	{
		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = KNOCKBACK_STATE;

		fsmData->world->server.SendData(&statePacket);
	}
}

void SNFSMAPKnockbackState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	timer += dt;

	if (timer >= dustDelay)
	{
		fsmData->world->particleSystem->StartParticleEffect(
			fsmData->autonomousProxy->transform.GetPosition(),
			fsmData->world->dustCloud01Anim, fsmData->world->dustCloud01Anim->duration, fsmData->autonomousProxy->transform.GetFacingRight());
		timer = 0.0f;
	}

	SNTransform* transform = &autoProxy->transform;

	transform->SetPreviousPosition(transform->GetPosition());
	transform->SetVelocity(transform->GetVelocity() + transform->GetAcceleration() * dt);
	transform->SetPosition(transform->GetPosition() + transform->GetVelocity() * dt);

	if (autoProxy->IsGrounded())
	{
		fsmData->autonomousProxy->animator->rotation = 0;
		autoProxy->EnterState(KNOCKDOWN_STATE);
	}
}

void SNFSMAPKnockbackState::Exit(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->transform.SetVelocity({ 0, 0 });
	fsmData->autonomousProxy->transform.SetAcceleration({ 0, 0 });
}
