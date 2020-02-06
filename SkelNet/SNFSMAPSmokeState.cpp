#include "SNFSMAPSmokeState.h"
#include "SNWorld.h"
#include "SNFSMData.h"
#include "SNAnimator.h"

void SNFSMAPSmokeState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->smokeAnim);
	tauntDuration = fsmData->world->smokeAnim->duration;
	timer = 0.0f;
	hasPufferSoundPlayed = false;
}

void SNFSMAPSmokeState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= tauntDuration)
	{
		fsmData->autonomousProxy->EnterState(IDLE_STATE);
	}
}

void SNFSMAPSmokeState::Exit(SNFSMData* fsmData)
{

}
