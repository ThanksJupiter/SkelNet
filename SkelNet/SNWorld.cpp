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

	particleSystem = new SNParticleSystem();

	trail = new SNTrail();
	SNSprite* trailSprite = new SNSprite(32, 32, nullptr, 0);
	trailSprite->texture = engLoadTexture("SN_Skel_Walk-Sheet.png");
	trail->Setup({ 70,70 }, 5, 10, 10, 5, *trailSprite);
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

	if (isServer)
	{
		server.RecvData();
	}
	else
	{
		client.RecvData();

		doStartup = GetFlag(client.recievedData.flags, 5);
		if (doStartup)
		{
			RestartGame();
			engDrawString({ 100, 100 }, "DoStartup");
			return;
		}
	}

	if (isServer)
	{
		Vector2 newPosition = Vector2(server.recievedData.posX, server.recievedData.posY);
		simulatedProxy.SetPosition(newPosition);
		simulatedProxy.health = server.recievedData.health;

		//autonomousProxy.serverAttacked = GetFlag(server.recievedData.flags, 0);
		//autonomousProxy.serverWasHit = GetFlag(server.recievedData.flags, 1);
		autonomousProxy.clientAttacked = GetFlag(server.recievedData.flags, 2);
		//autonomousProxy.clientWasHit = GetFlag(server.recievedData.flags, 3);

		if (simulatedProxy.animState != server.recievedData.animState && server.recievedData.animState < 7)
		{
			simulatedProxy.animState = server.recievedData.animState;
			simulatedProxy.SetAnimation(simulatedProxy.animState);
		}

		if (autonomousProxy.clientAttacked)
		{
			simulatedProxy.PlayAttackAnim();
			autonomousProxy.clientAttacked = false;
		}
	}
	else
	{
		Vector2 newPosition = Vector2(client.recievedData.posX, client.recievedData.posY);
		simulatedProxy.SetPosition(newPosition);
		simulatedProxy.health = client.recievedData.health;

		autonomousProxy.serverAttacked = GetFlag(client.recievedData.flags, 0);
		autonomousProxy.serverWasHit = GetFlag(client.recievedData.flags, 1);
		//autonomousProxy.clientAttacked = GetFlag(client.recievedData.flags, 2);
		autonomousProxy.clientWasHit = GetFlag(client.recievedData.flags, 3);

		if (simulatedProxy.animState != client.recievedData.animState && client.recievedData.animState < 7)
		{
			simulatedProxy.animState = client.recievedData.animState;
			simulatedProxy.SetAnimation(simulatedProxy.animState);
		}


		if (autonomousProxy.clientWasHit)
		{
			autonomousProxy.TakeDamage();
			autonomousProxy.clientWasHit = false;
		}
		if (autonomousProxy.serverWasHit)
		{
			simulatedProxy.TakeDamage();
			autonomousProxy.serverWasHit = false;
		}
		if (autonomousProxy.serverAttacked)
		{
			simulatedProxy.PlayAttackAnim();
			autonomousProxy.serverAttacked = false;
		}
	}

	if (isServer)
	{
		if (autonomousProxy.position.x >= (worldSize.x / 2) + deathDistance.x || 
			autonomousProxy.position.x <= (worldSize.x / 2) - deathDistance.x ||
			autonomousProxy.position.y >= (worldSize.y / 2) + deathDistance.y || 
			autonomousProxy.position.y <= (worldSize.y / 2) - deathDistance.y)
		{
			RestartGame();
			return;
		}
		
		if (simulatedProxy.position.x >= (worldSize.x / 2) + deathDistance.x ||
			simulatedProxy.position.x <= (worldSize.x / 2) - deathDistance.x ||
			simulatedProxy.position.y >= (worldSize.y / 2) + deathDistance.y ||
			simulatedProxy.position.y <= (worldSize.y / 2) - deathDistance.y)
		{
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
}

void SNWorld::SpawnSimulatedProxy(SNWorld& worldptr)
{
	Vector2 initPos;
	initPos.x = worldSize.x / 2;
	initPos.y = (worldSize.y / 3) * 2;

	simulatedProxy.Spawn(initPos, worldptr);
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

void SNWorld::SendPlayerData(Vector2 position, int health, bool serverAttacked, bool serverWasHit, bool clientAttacked, bool clientWasHit)
{
	if (isServer)
	{
		server.statePack.posX = position.x;
		server.statePack.posY = position.y;
		server.statePack.id = 1;
		server.statePack.health = health;

		if (serverAttacked)
			SetFlag(server.statePack.flags, 0);
		else
			UnsetFlag(server.statePack.flags, 0);

		autonomousProxy.serverAttacked = false;

		if (serverWasHit)
			SetFlag(server.statePack.flags, 1);
		else
			UnsetFlag(server.statePack.flags, 1);

		/*if (clientAttacked)
			SetFlag(server.statePack.flags, 2);
		else
			UnsetFlag(server.statePack.flags, 2);*/

		if (clientWasHit)
			SetFlag(server.statePack.flags, 3);
		else
			UnsetFlag(server.statePack.flags, 3);

		server.SendData();
		UnsetFlag(server.statePack.flags, 5);
	}
	else
	{
		client.statePack.posX = position.x;
		client.statePack.posY = position.y;
		client.statePack.id = 0;
		client.statePack.health = health;

		/*if (serverAttacked)
			SetFlag(client.statePack.flags, 0);
		else
			UnsetFlag(client.statePack.flags, 0);*/

			/*if (serverWasHit)
				SetFlag(client.statePack.flags, 1);
			else
				UnsetFlag(client.statePack.flags, 1);*/

		if (clientAttacked)
			SetFlag(client.statePack.flags, 2);
		else
			UnsetFlag(client.statePack.flags, 2);

		/*if (clientWasHit)
			SetFlag(client.statePack.flags, 3);
		else
			UnsetFlag(client.statePack.flags, 3);*/

		client.SendData();
	}
}

void SNWorld::RestartGame()
{
	//Reset damage
	//Reset positions
	//Reset animations
	//Reset UI
	if (isServer)
	{
		SetFlag(server.statePack.flags, 5);
	}

	simulatedProxy.Reset();
	autonomousProxy.Reset();
}