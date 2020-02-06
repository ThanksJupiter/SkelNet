#include "SNFSMAPJumpAscendState.h"
#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNParticleSystem.h"
#include "SNEngine.h"

void SNFSMAPJumpAscendState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->somersaultAnim);

	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->autonomousProxy->transform.GetPosition(),
		fsmData->world->vortexDustAnim, fsmData->world->vortexDustAnim->duration, fsmData->autonomousProxy->transform.GetFacingRight());

	vortexTimer = 0.0f;
	timer = 0.0f;
}

void SNFSMAPJumpAscendState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	vortexTimer += dt;
	timer += dt;

	if (timer >= maxAscendTime || autoProxy->transform.GetVelocity().y > 0.0f)
	{
		autoProxy->EnterState(FALL_STATE);
		return;
	}

	if (vortexTimer >= vortexDelay)
	{
		fsmData->world->particleSystem->StartParticleEffect(
			fsmData->autonomousProxy->transform.GetPosition(),
			fsmData->world->vortexDustAnim, fsmData->world->vortexDustAnim->duration, fsmData->autonomousProxy->transform.GetFacingRight(), 3, angles[rand() % 4]);
		vortexTimer = 0.0f;
	}

	if (input->attack)
	{
		autoProxy->EnterState(ATTACK_STATE);
		return;
	}

	autoProxy->transform.SetAcceleration({ autoProxy->accelerationSpeed * autoProxy->airControlMult * input->leftStickDirection.x, autoProxy->gravity * input->jumpHeld ? autoProxy->gravityMult : autoProxy->fallGravityMult });

	if (!input->jumpHeld)
	{
		autoProxy->EnterState(FALL_STATE);
		return;
	}

	autoProxy->ForcesTimeIntegration(dt);
}

void SNFSMAPJumpAscendState::Exit(SNFSMData* fsmData)
{

}
