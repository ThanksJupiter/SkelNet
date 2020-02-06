#include "SNFSMAPJumpSquatState.h"
#include "SNAutonomousProxy.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNParticleSystem.h"
#include "SNEngine.h"

void SNFSMAPJumpSquatState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->jumpSquatAnim);

	timer = 0.0f;
	jumpDelay = fsmData->world->jumpSquatAnim->duration;
	fsmData->autonomousProxy->transform.SetAcceleration({ 0, 0 });
	fsmData->autonomousProxy->transform.SetVelocity({ 0, 0 });
}

void SNFSMAPJumpSquatState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= jumpDelay)
	{
		fsmData->autonomousProxy->EnterState(JUMP_START_STATE);
	}
}

void SNFSMAPJumpSquatState::Exit(SNFSMData* fsmData)
{
	
}
