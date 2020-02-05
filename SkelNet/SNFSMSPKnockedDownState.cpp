#include "SNFSMSPKnockedDownState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMSPKnockedDownState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->knockedDownAnim);

	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->simulatedProxy->transform.GetPosition(),
		fsmData->world->landingDustAnim, fsmData->world->landingDustAnim->duration, fsmData->simulatedProxy->transform.GetFacingRight());
}

void SNFSMSPKnockedDownState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPKnockedDownState::Exit(SNFSMData* fsmData)
{

}
