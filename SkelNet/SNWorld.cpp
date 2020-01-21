#include "SNWorld.h"
#include "SNEngine.h"
#include "SNDataPacket.h"
#include <math.h>
#include <vector>
#include "SNAnimator.h"

void SNWorld::Update()
{
	autonomousProxy.Update();

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
	}

	if (isServer)
	{
		Vector2 newPosition = Vector2(server.recievedData.posX, server.recievedData.posY);
		simulatedProxy.SetPosition(newPosition);
		simulatedProxy.health = server.recievedData.health;

		autonomousProxy.serverAttacked = server.recievedData.serverAttacked;
		autonomousProxy.serverWasHit = server.recievedData.serverWasHit;
		autonomousProxy.clientAttacked = server.recievedData.clientAttacked;
		autonomousProxy.clientWasHit = server.recievedData.clientWasHit;
	}
	else
	{
		Vector2 newPosition = Vector2(client.recievedData.posX, client.recievedData.posY);
		simulatedProxy.SetPosition(newPosition);
		simulatedProxy.health = client.recievedData.health;

		autonomousProxy.serverAttacked = client.recievedData.serverAttacked;
		autonomousProxy.serverWasHit = client.recievedData.serverWasHit;
		autonomousProxy.clientAttacked = client.recievedData.clientAttacked;
		autonomousProxy.clientWasHit = client.recievedData.clientWasHit;

		if (client.recievedData.clientWasHit == 1)
		{
			autonomousProxy.TakeDamage();
		}
		if (client.recievedData.serverWasHit == 1)
		{
			simulatedProxy.TakeDamage();
		}
		if (client.recievedData.serverAttacked == 1)
		{
			simulatedProxy.PlayAttackAnim();
		}
	}
}

void SNWorld::Draw(float dt)
{
	autonomousProxy.Draw(dt);
	simulatedProxy.Draw(dt);

	for (int i = 0; i < numHitboxes; ++i)
	{
		hitboxes[i].DrawDebug();
	}

	floors[0].Draw();
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

SNHitBox* SNWorld::SpawnHitBox(Vector2 position, Vector2 size, Vector2 offset, bool blocking, bool callDelegates, std::function<void()> OnTriggerEnter, std::function<void()> OnTriggerExit)
{
	hitboxes[numHitboxes].Setup(position, size, offset, blocking, callDelegates, OnTriggerEnter, OnTriggerExit);
	hitboxes[numHitboxes].drawDebug = true;

	numHitboxes++;

	return &hitboxes[numHitboxes - 1];
}

void SNWorld::SendPlayerData(Vector2 position, int health, bool serverAttacked, bool serverWasHit, bool clientAttacked, bool clientWasHit)
{
	if (!isServer)
	{
		client.statePack.posX = position.x;
		client.statePack.posY = position.y;
		client.statePack.id = 0;
		client.statePack.health = health;

		client.statePack.serverAttacked = serverAttacked ? 1 : 0;
		client.statePack.serverWasHit = serverWasHit ? 1 : 0;
		client.statePack.clientAttacked = clientAttacked ? 1 : 0;
		client.statePack.clientWasHit = clientWasHit ? 1 : 0;

		client.SendData();
	}
	else
	{
		server.statePack.posX = position.x;
		server.statePack.posY = position.y;
		server.statePack.id = 1;
		server.statePack.health = health;

		server.statePack.serverAttacked = serverAttacked ? 1 : 0;
		server.statePack.serverWasHit = serverWasHit ? 1 : 0;
		server.statePack.clientAttacked = clientAttacked ? 1 : 0;
		server.statePack.clientWasHit = clientWasHit ? 1 : 0;

		server.SendData();
	}
}

void SNWorld::CheckCollisions()
{

}
