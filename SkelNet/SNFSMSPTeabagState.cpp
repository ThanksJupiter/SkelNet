#include "SNFSMSPTeabagState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMSPTeabagState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->teabagAnim);
	timer = 0.0f;
	hasPufferSoundPlayed = false;
}

void SNFSMSPTeabagState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= pufferDelay && !hasPufferSoundPlayed)
	{
		SNSimulatedProxy* simulProxy = fsmData->simulatedProxy;
		fsmData->world->particleSystem->StartParticleEffect(
			simulProxy->transform.GetPosition(),
			fsmData->world->coolDustAnim, fsmData->world->coolDustAnim->duration, simulProxy->transform.GetFacingRight());

		fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->punch);
		hasPufferSoundPlayed = true;
	}
}

void SNFSMSPTeabagState::Exit(SNFSMData* fsmData)
{
	
}
