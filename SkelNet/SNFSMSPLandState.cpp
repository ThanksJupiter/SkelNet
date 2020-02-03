#include "SNFSMSPLandState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMSPLandState::Enter(SNFSMData* fsmData)
{
	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->simulatedProxy->transform.GetPosition(),
		fsmData->world->landingDustAnim, 8 * 0.05f, fsmData->simulatedProxy->transform.GetFacingRight());

	fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->land);
}

void SNFSMSPLandState::Update(float dt, SNFSMData* fsmData)
{
	
}

void SNFSMSPLandState::Exit(SNFSMData* fsmData)
{
	
}
