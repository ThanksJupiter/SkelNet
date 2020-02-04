#include "SNFSMSPDeathState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"
#include "SNTransform.h"
#include "SNCamera.h"

void SNFSMSPDeathState::Enter(SNFSMData* fsmData)
{
	Vector2 pos = fsmData->simulatedProxy->transform.GetPosition();
	pos.y += (128 * 6) / 2;

	fsmData->world->particleSystem->StartParticleEffect(
		pos,
		fsmData->world->deathShockwave,
		fsmData->world->deathShockwave->frameCount * fsmData->world->deathShockwave->nextFrameDelay,
		false, 6, 0);

	fsmData->simulatedProxy->currentStocks--;
}

void SNFSMSPDeathState::Update(float dt, SNFSMData* fsmData)
{
	
}

void SNFSMSPDeathState::Exit(SNFSMData* fsmData)
{
	
}
