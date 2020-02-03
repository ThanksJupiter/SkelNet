#include "SNFSMSPAttackState.h"
#include "SNWorld.h"
#include "SNAnimator.h"
#include "SNFSMData.h"

void SNFSMSPAttackState::Enter(SNFSMData* fsmData)
{
	timer = 0.f;
	hit = false;
	fsmData->simulatedProxy->animator->SetCurrentAnimation(fsmData->world->spAttackAnim);
}

void SNFSMSPAttackState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= checkAttackDuration && !hit)
	{
		hit = true;
		fsmData->simulatedProxy->DoAttack();
	}
}

void SNFSMSPAttackState::Exit(SNFSMData* fsmData)
{
	
}
