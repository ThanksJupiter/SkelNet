#include "SNFSMSPKnockbackState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMSPKnockbackState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->TakeDamage();
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->knockbackAnim);
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
