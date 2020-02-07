#include "SNFSMAPDootState.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNWorld.h"

void SNFSMAPDootState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->dootAnim);

	dootDuration = fsmData->world->dootAnim->duration;
	timer = 0.0f;
}

void SNFSMAPDootState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= dootDuration)
	{
		fsmData->autonomousProxy->EnterState(IDLE_STATE);
	}
}

void SNFSMAPDootState::Exit(SNFSMData* fsmData)
{

}
