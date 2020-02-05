#include "SNFSMSPKnockbackState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"
#include <string>
#include "SNEngine.h"

void SNFSMSPKnockbackState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->TakeDamage();
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->knockbackAnim);

	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->simulatedProxy->transform.GetPosition(),
		fsmData->world->hitEffect01, fsmData->world->hitEffect01->duration, fsmData->simulatedProxy->transform.GetFacingRight());

	std::string healthString = std::to_string(fsmData->simulatedProxy->health);
	healthString.push_back('%');
	fsmData->world->simProxyHealthText->UpdateText(healthString);
	fsmData->world->simProxyHealthText->SetRelativePosition(
		{ -engGetTextSize(healthString.c_str()).x * 2,
		0 });
}

void SNFSMSPKnockbackState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNSimulatedProxy* simProxy = fsmData->simulatedProxy;

	timer += dt;

	if (timer >= dustDelay)
	{
		fsmData->world->particleSystem->StartParticleEffect(
			simProxy->transform.GetPosition(),
			fsmData->world->dustCloud01Anim, fsmData->world->dustCloud01Anim->duration, simProxy->transform.GetFacingRight());
		timer = 0.0f;
	}
}

void SNFSMSPKnockbackState::Exit(SNFSMData* fsmData)
{

}
