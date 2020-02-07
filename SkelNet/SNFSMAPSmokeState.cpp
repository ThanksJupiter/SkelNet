#include "SNFSMAPSmokeState.h"
#include "SNWorld.h"
#include "SNFSMData.h"
#include "SNAnimator.h"

void SNFSMAPSmokeState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->smokeAnim);

	tauntDuration = fsmData->world->smokeAnim->duration;
	timer = 0.0f;
	pufferDelay = fsmData->world->smokeAnim->nextFrameDelay * 4.5f;
	hasSmokeSoundPlayed = false;
}

void SNFSMAPSmokeState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= pufferDelay && !hasSmokeSoundPlayed)
	{
		fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->smokeThrowSound);
		hasSmokeSoundPlayed = true;
	}

	if (timer >= tauntDuration)
	{
		fsmData->autonomousProxy->EnterState(IDLE_STATE);
	}
}

void SNFSMAPSmokeState::Exit(SNFSMData* fsmData)
{

}
