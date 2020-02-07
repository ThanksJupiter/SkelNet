#include "SNFSMSPAttackState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"

void SNFSMSPAttackState::Enter(SNFSMData* fsmData)
{
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->spAttackAnim);
	timer = 0.f;
	hit = false;
	hasMissSoundPlayed = false;
	hasStartSoundPlayed = false;
}

void SNFSMSPAttackState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= checkAttackDuration && !hit)
	{
		hit = true;
		fsmData->simulatedProxy->DoAttack();
	}

	if (timer >= startupSoundDelay && !hasStartSoundPlayed)
	{
		fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->whip_start);
		hasStartSoundPlayed = true;
	}

	if (timer >= missSoundDelay && !hasMissSoundPlayed)
	{
		fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->whip_miss);
		hasMissSoundPlayed = true;
	}
}

void SNFSMSPAttackState::Exit(SNFSMData* fsmData)
{
	
}
