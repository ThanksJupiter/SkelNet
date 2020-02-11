#include "SNFSMAPDashAttackState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"


void SNFSMAPDashAttackState::Enter(SNFSMData* fsmData)
{
	SNSimulatedProxy* autoProxy = fsmData->simulatedProxy;
	SNInput* input = fsmData->input;

	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->dashTackleAnim);

	autoProxy->transform.SetVelocity({ 310.0f * input->leftStickDirection.x, autoProxy->transform.GetPosition().y });

	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->autonomousProxy->transform.GetPosition(),
		fsmData->world->dashDustAnim, 8 * 0.05f, fsmData->autonomousProxy->transform.GetFacingRight());

	hasMissSoundPlayed = false;
	hasStartSoundPlayed = false;

	attackDuration = fsmData->world->dashTackleAnim->duration;
	timer = 0.0f;
	fsmData->autonomousProxy->SendEnterAttackState();
	hit = false;
}

void SNFSMAPDashAttackState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	timer += dt;

	autoProxy->ForcesTimeIntegration(dt);

	/*if (timer >= checkAttackDuration && !hit)
	{
		hit = true;

		if (fsmData->world->HasAuthority())
		{
			autoProxy->DoAttack();
		}
		else
		{
			//SPDoAttack(fsmData->world);
		}
	}*/

	if (timer >= attackDuration)
	{
		autoProxy->EnterState(IDLE_STATE);
	}
}

void SNFSMAPDashAttackState::Exit(SNFSMData* fsmData)
{
	
}
