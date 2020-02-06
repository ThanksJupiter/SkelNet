#include "SNFSMSPRunState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMSPRunState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->runAnim);

	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->simulatedProxy->transform.GetPosition(),
		fsmData->world->dashDustAnim, 8 * 0.05f, !fsmData->simulatedProxy->transform.GetFacingRight());
}

void SNFSMSPRunState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPRunState::Exit(SNFSMData* fsmData)
{

}
