#include "SNFSMSPJumpAscendState.h"
#include "SNParticleSystem.h"
#include "SNFSMData.h"
#include "SNAnimation.h"
#include "SNSimulatedProxy.h"
#include "SNWorld.h"

void SNFSMSPJumpAscendState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->somersaultAnim);

	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->simulatedProxy->transform.GetPosition(),
		fsmData->world->vortexDustAnim, fsmData->world->vortexDustAnim->duration, fsmData->autonomousProxy->transform.GetFacingRight());

	vortexTimer = 0.0f;
}

void SNFSMSPJumpAscendState::Update(float dt, SNFSMData* fsmData)
{
	vortexTimer += dt;

	if (vortexTimer >= vortexDelay)
	{
		fsmData->world->particleSystem->StartParticleEffect(
			fsmData->simulatedProxy->transform.GetPosition(),
			fsmData->world->vortexDustAnim, fsmData->world->vortexDustAnim->duration, fsmData->autonomousProxy->transform.GetFacingRight(), 3, angles[rand() % 4]);
		vortexTimer = 0.0f;
	}
}

void SNFSMSPJumpAscendState::Exit(SNFSMData* fsmData)
{

}
