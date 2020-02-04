#include "SNFSMAPDeathState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"
#include "SNTransform.h"

void SNFSMAPDeathState::Enter(SNFSMData* fsmData)
{
	fsmData->world->particleSystem->StartParticleEffect(
		fsmData->autonomousProxy->transform.GetPosition() -
		fsmData->autonomousProxy->transform.GetScale() / 2,
		fsmData->world->deathShockwave,
		fsmData->world->deathShockwave->frameCount * fsmData->world->deathShockwave->nextFrameDelay,
		false, 6, 0);

	fsmData->autonomousProxy->currentStocks--;
}

void SNFSMAPDeathState::Update(float dt, SNFSMData* fsmData)
{
	fsmData->autonomousProxy->ForcesTimeIntegration(dt);
}

void SNFSMAPDeathState::Exit(SNFSMData* fsmData)
{
	
}
