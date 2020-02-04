#include "SNFSMSPDeathState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"
#include "SNTransform.h"

void SNFSMSPDeathState::Enter(SNFSMData* fsmData)
{
	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->simulatedProxy->transform.GetPosition() -
		fsmData->simulatedProxy->transform.GetScale() / 2,
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
