#include "SNFSMAPTurnAroundState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMAPTurnAroundState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->turnAroundAnim);

	flipDuration = fsmData->world->turnAroundAnim->frameCount * fsmData->world->turnAroundAnim->nextFrameDelay;

	timer = 0.0f;
}

void SNFSMAPTurnAroundState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;
	
	if (timer >= flipDuration)
	{
		SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
		autoProxy->EnterState(IDLE_STATE);
	}
}

void SNFSMAPTurnAroundState::Exit(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	autoProxy->flip = !autoProxy->flip;

	autoProxy->animator->direction = autoProxy->flip? -1 : 1;
}
