#include "SNWorld.h"
#include "SNEngine.h"
#include "SNDataPacket.h"
#include <math.h>
#include <vector>
#include "SNAnimator.h"
#include "SNFlags.h"
#include "SNTrailRenderer.h"
#include "SNParticleSystem.h"

void SNWorld::Setup()
{
	audioManager = new SNAudioManager;
	audioManager->InitSounds();
	//audioManager->PlayMusicLoop(audioManager->midnaLament);

	particleSystem = new SNParticleSystem();

	trail = new SNTrail();
	SNSprite* trailSprite = new SNSprite(32, 32, nullptr, 0);
	trailSprite->texture = engLoadTexture("SN_Skel_Walk-Sheet.png");
	trail->Setup({ 70,70 }, 5, 10, 10, 5, *trailSprite);

	client.world = this;
	server.world = this;
}

void SNWorld::Update(float dt)
{
	autonomousProxy.Update(dt);

	for (int i = 0; i < numHitboxes; ++i)
	{
		hitboxes[i].lastState = hitboxes[i].currentState;
		hitboxes[i].currentState.isTriggered = false;
		hitboxes[i].currentState.frameNum = engGetFrameNum();
	}

	for (int i = 0; i < numHitboxes; ++i)
	{
		for (int j = 0; j < numHitboxes; ++j)
		{
			if (i == j)
				continue;

			hitboxes[i].CheckCollision(hitboxes[j]);
		}
	}

	if (HasAuthority())
	{
		server.RecvData();
	}
	else
	{
		client.RecvData();
	}

	if (HasAuthority())
	{
		if (autonomousProxy.transform.GetPosition().x >= (worldSize.x / 2) + deathDistance.x || 
			autonomousProxy.transform.GetPosition().x <= (worldSize.x / 2) - deathDistance.x ||
			autonomousProxy.transform.GetPosition().y >= (worldSize.y / 2) + deathDistance.y ||
			autonomousProxy.transform.GetPosition().y <= (worldSize.y / 2) - deathDistance.y)
		{
			particleSystem->StartParticleEffect(autonomousProxy.transform.GetPosition(), dashDustAnim, 8 * 0.05f, false, 10, 45.f);
			RestartGame();
			return;
		}
		
		if (simulatedProxy.transform.GetPosition().x == 0)
		{
			return;
		}

		if (simulatedProxy.transform.GetPosition().x >= (worldSize.x / 2) + deathDistance.x ||
			simulatedProxy.transform.GetPosition().x <= (worldSize.x / 2) - deathDistance.x ||
			simulatedProxy.transform.GetPosition().y >= (worldSize.y / 2) + deathDistance.y ||
			simulatedProxy.transform.GetPosition().y <= (worldSize.y / 2) - deathDistance.y)
		{
			particleSystem->StartParticleEffect(simulatedProxy.transform.GetPosition(), dashDustAnim, 8 * 0.05f, false, 10, 45.f);
			RestartGame();
			return;
		}
	}
}

void SNWorld::Draw(float dt)
{
	simulatedProxy.Draw(dt);
	autonomousProxy.Draw(dt);

	for (int i = 0; i < numHitboxes; ++i)
	{
		//hitboxes[i].DrawDebug();
	}

	//floors[0].Draw();

	if (particleSystem)
	{
		particleSystem->UpdateParticles(dt);
	}

	float width = worldSize.x / 2;

	SDL_Rect dstRect =
	{
		(worldSize.x / 2) - (levelSprite->width * 3) / 2,
		12,//worldSize.x,
		levelSprite->width * 3,
		levelSprite->height * 3,
	};

	engDrawSprite(levelSprite->sheetSourceRect, dstRect, levelSprite->texture);

	trail->Draw();

	if (engGetKey(Key::R))
	{
		engSetColor(255, 0, 0);
		engDrawLine((worldSize / 2) - deathDistance, { (worldSize / 2).x + deathDistance.x, (worldSize / 2).y - deathDistance.y });
		engDrawLine((worldSize / 2) - deathDistance, { (worldSize / 2).x - deathDistance.x, (worldSize / 2).y + deathDistance.y });
		engDrawLine((worldSize / 2) + deathDistance, { (worldSize / 2).x + deathDistance.x, (worldSize / 2).y - deathDistance.y });
		engDrawLine((worldSize / 2) + deathDistance, { (worldSize / 2).x - deathDistance.x, (worldSize / 2).y + deathDistance.y });
		engSetColor(0, 0, 0);
	}
}

void SNWorld::SpawnAutonomousProxy(SNWorld& worldptr)
{
	Vector2 initPos;
	initPos.x = worldSize.x / 2;
	initPos.y = (worldSize.y / 3) * 2;

	autonomousProxy = SNAutonomousProxy();
	autonomousProxy.Spawn({ initPos.x + 50, initPos.y }, worldptr);


	if (isServer)
	{
		autonomousProxy.animator->r = 155;
		autonomousProxy.animator->g = 155;
		autonomousProxy.animator->b = 255;
	}
	else
	{
		autonomousProxy.animator->r = 255;
		autonomousProxy.animator->g = 155;
		autonomousProxy.animator->b = 155;
	}
}

void SNWorld::SpawnSimulatedProxy(SNWorld& worldptr)
{
	Vector2 initPos;
	initPos.x = worldSize.x / 2;
	initPos.y = (worldSize.y / 3) * 2;

	simulatedProxy.Spawn(initPos, worldptr);

	if (isServer)
	{
		simulatedProxy.animator->r = 255;
		simulatedProxy.animator->g = 155;
		simulatedProxy.animator->b = 155;
	}
	else
	{
		simulatedProxy.animator->r = 155;
		simulatedProxy.animator->g = 155;
		simulatedProxy.animator->b = 255;
	}
}

void SNWorld::SpawnFloor(Vector2 position, Vector2 size)
{
	floors[0].position = position;
	floors[0].size = size;
}

SNHitBox* SNWorld::SpawnHitBox(Vector2 position, Vector2 size, Vector2 offset, char id, bool blocking, bool callDelegates, std::function<void()> OnTriggerEnter, std::function<void()> OnTriggerExit)
{
	hitboxes[numHitboxes].Setup(position, size, offset, id, blocking, callDelegates, OnTriggerEnter, OnTriggerExit);
	hitboxes[numHitboxes].drawDebug = true;

	numHitboxes++;

	return &hitboxes[numHitboxes - 1];
}

bool SNWorld::HasAuthority()
{
	return isServer;
}

void SNWorld::RestartGame()
{
	//Reset damage
	//Reset positions
	//Reset animations
	//Reset UI
	if (HasAuthority())
	{
		// Send ResetGame Flag
		//SetFlag(server.statePack.flags, 5);
	}

	simulatedProxy.Reset();
	autonomousProxy.Reset();
}