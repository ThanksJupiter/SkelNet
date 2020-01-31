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
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	timer += dt;
	
	if (timer >= flipDuration)
	{
		autoProxy->EnterState(IDLE_STATE);
	}

	if (input->attack)
	{
		autoProxy->EnterState(ATTACK_STATE);
	}

	if (input->jump)
	{
		autoProxy->EnterState(JUMP_STATE);
	}
}

void SNFSMAPTurnAroundState::Exit(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	autoProxy->transform.SetFacingRight(!autoProxy->transform.GetFacingRight());
	autoProxy->SetDirection();
}
