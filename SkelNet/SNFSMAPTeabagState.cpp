#include "SNFSMAPTeabagState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"

void SNFSMAPTeabagState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->teabagAnim);
	timer = 0.0f;
	hasPufferSoundPlayed = false;
}

void SNFSMAPTeabagState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	timer += dt;

	if (timer >= tauntDuration)
	{
		autoProxy->EnterState(IDLE_STATE);
	}

	if (timer >= pufferDelay && !hasPufferSoundPlayed)
	{
		fsmData->world->particleSystem->StartParticleEffect(
			fsmData->autonomousProxy->transform.GetPosition(),
			fsmData->world->coolDustAnim, fsmData->world->coolDustAnim->duration, fsmData->autonomousProxy->transform.GetFacingRight());

		fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->punch);
		hasPufferSoundPlayed = true;
	}
}

void SNFSMAPTeabagState::Exit(SNFSMData* fsmData)
{
	
}
