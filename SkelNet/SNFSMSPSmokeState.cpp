#include "SNFSMSPSmokeState.h"
#include "SNFSMData.h"
#include "SNAnimator.h"
#include "SNWorld.h"


void SNFSMSPSmokeState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->smokeAnim);

	pufferDelay = fsmData->world->smokeAnim->nextFrameDelay * 4.5f;
	timer = 0.0f;
	hasSmokeSoundPlayed = false;
}

void SNFSMSPSmokeState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= pufferDelay && !hasSmokeSoundPlayed)
	{
		fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->smokeThrowSound);
		hasSmokeSoundPlayed = true;
	}
}

void SNFSMSPSmokeState::Exit(SNFSMData* fsmData)
{

}
