#include "SNFSMAPTauntState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"

void SNFSMAPTauntState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->teabagAnim);
	timer = 0.0f;
	hasPufferSoundPlayed = false;
}

void SNFSMAPTauntState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	timer += dt;

	if (timer >= tauntDuration)
	{
		autoProxy->EnterState(IDLE_STATE);
	}

	if (timer >= pufferDelay && !hasPufferSoundPlayed)
	{
		fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->punch);
		hasPufferSoundPlayed = true;
	}
}

void SNFSMAPTauntState::Exit(SNFSMData* fsmData)
{
	
}
