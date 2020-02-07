#include "SNFSMSPDeathState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"
#include "SNTransform.h"
#include "SNEngine.h"

void SNFSMSPDeathState::Enter(SNFSMData* fsmData)
{
	Vector2 pos = fsmData->simulatedProxy->transform.GetPosition();
	pos.y += (128 * 6) / 2;

	fsmData->world->particleSystem->StartParticleEffect(
		pos,
		fsmData->world->deathShockwave,
		fsmData->world->deathShockwave->frameCount * fsmData->world->deathShockwave->nextFrameDelay,
		false, 6, 0);

	fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->deathSound);

	if (fsmData->simulatedProxy->currentStocks > 0)
	{
		fsmData->simulatedProxy->currentStocks--;

		if (fsmData->world->HasAuthority())
		{
			SNHealthPacket healthPacket;
			healthPacket.flag = SP_HEALTH_FLAG;
			healthPacket.health = fsmData->simulatedProxy->health;
			healthPacket.stocks = fsmData->simulatedProxy->currentStocks;

			fsmData->world->server.SendData(&healthPacket);
		}
	}
	fsmData->simulatedProxy->health = 0.f;
	fsmData->world->simProxyHealthText->UpdateText("0%");
	fsmData->world->simProxyStockText->UpdateText(fsmData->simulatedProxy->currentStocks);
	fsmData->world->simProxyHealthText->SetRelativePosition(
		{ -engGetTextSize("0%").x * 2,
		0 });
}

void SNFSMSPDeathState::Update(float dt, SNFSMData* fsmData)
{

}

void SNFSMSPDeathState::Exit(SNFSMData* fsmData)
{

}
