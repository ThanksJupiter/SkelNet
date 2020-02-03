#include "SNFSMAPLandState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMAPLandState::Enter(SNFSMData* fsmData)
{
	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->autonomousProxy->transform.GetPosition(),
		fsmData->world->landingDustAnim, 8 * 0.05f, fsmData->autonomousProxy->transform.GetFacingRight());

	fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->land);

	fsmData->autonomousProxy->transform.SetVelocity({0, 0});
}

void SNFSMAPLandState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= landingDelay)
	{
		fsmData->autonomousProxy->SetState(IDLE_STATE);
	}
}

void SNFSMAPLandState::Exit(SNFSMData* fsmData)
{
	
}
