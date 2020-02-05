#include "SNFSMSPTauntState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMSPTauntState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->teabagAnim);
}

void SNFSMSPTauntState::Update(float dt, SNFSMData* fsmData)
{
	SNSimulatedProxy* simulProxy = fsmData->simulatedProxy;
	timer += dt;

	if (timer >= pufferDelay && !hasPufferSoundPlayed)
	{
		fsmData->world->particleSystem->StartParticleEffect(
			simulProxy->transform.GetPosition(),
			fsmData->world->coolDustAnim, fsmData->world->coolDustAnim->duration, simulProxy->transform.GetFacingRight());

		fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->punch);
		hasPufferSoundPlayed = true;
	}
}

void SNFSMSPTauntState::Exit(SNFSMData* fsmData)
{

}
