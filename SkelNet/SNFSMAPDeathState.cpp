#include "SNFSMAPDeathState.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNParticleSystem.h"
#include "SNTransform.h"
#include "SNEngine.h"

void SNFSMAPDeathState::Enter(SNFSMData* fsmData)
{
	Vector2 pos = fsmData->autonomousProxy->transform.GetPosition();
	pos.y += (128 * 6) / 2;

	fsmData->world->particleSystem->StartParticleEffect(
		pos,
		fsmData->world->deathShockwave,
		fsmData->world->deathShockwave->frameCount * fsmData->world->deathShockwave->nextFrameDelay,
		false, 6, 0);

	fsmData->world->audioManager->PlayChunkOnce(fsmData->world->audioManager->deathExplosionSound);

	if (fsmData->autonomousProxy->currentStocks > 0)
	{
		fsmData->autonomousProxy->currentStocks--;
		if (fsmData->world->HasAuthority())
		{
			SNHealthPacket healthPacket;
			healthPacket.flag = AP_HEALTH_FLAG;
			healthPacket.health = fsmData->autonomousProxy->health;
			healthPacket.stocks = fsmData->autonomousProxy->currentStocks;

			fsmData->world->server.SendData(&healthPacket);
		}
	}

	fsmData->autonomousProxy->health = 0.f;
	fsmData->world->autoProxyHealthText->UpdateText("0%");
	fsmData->world->autoProxyStockText->UpdateText(fsmData->autonomousProxy->currentStocks);
	fsmData->world->autoProxyHealthText->SetRelativePosition(
		{ -engGetTextSize("0%").x * 2,
		0 });
}

void SNFSMAPDeathState::Update(float dt, SNFSMData* fsmData)
{
	fsmData->autonomousProxy->ForcesTimeIntegration(dt);
}

void SNFSMAPDeathState::Exit(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->transform.SetVelocity({0, 0});
	fsmData->autonomousProxy->transform.SetAcceleration({ 0, 0 });
}
