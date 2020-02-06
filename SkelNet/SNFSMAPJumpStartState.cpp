#include "SNFSMAPJumpStartState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNParticleSystem.h"
#include "SNEngine.h"

void SNFSMAPJumpStartState::Enter(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->jumpAnim);

	autoProxy->world->audioManager->PlayChunkOnce(autoProxy->world->audioManager->jump);

	autoProxy->transform.SetVelocity({ 100.0f * fsmData->input->leftStickDirection.x, -200.0f });
	
	animationIsDone = fsmData->world->jumpAnim->duration;
	timer = 0.0f;
}

void SNFSMAPJumpStartState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	timer += dt;

	if (timer >= animationIsDone)
	{
		if (input->jumpHeld)
		{
			autoProxy->EnterState(JUMP_ASCEND_STATE);
			return;
		}
		else
		{
			autoProxy->EnterState(FALL_STATE);
			return;
		}
	}

	autoProxy->transform.SetAcceleration({ autoProxy->accelerationSpeed * autoProxy->airControlMult * input->leftStickDirection.x, autoProxy->gravity * input->jumpHeld ? autoProxy->gravityMult : autoProxy->fallGravityMult });

	if (input->attack)
	{
		autoProxy->EnterState(ATTACK_STATE);
		return;
	}

	autoProxy->ForcesTimeIntegration(dt);
}

void SNFSMAPJumpStartState::Exit(SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	autoProxy->animator->doManualAnimationCycling = false;
}
