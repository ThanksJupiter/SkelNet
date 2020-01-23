#include "SNWorld.h"
#include "SNEngine.h"
#include "SNDataPacket.h"
#include <math.h>
#include <vector>
#include "SNAnimator.h"
#include "SNFlags.h"

void SNWorld::Setup()
{
	audioManager = new SNAudioManager;
	audioManager->InitSounds();
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
}

void SNWorld::Draw(float dt)
{
	autonomousProxy.Draw(dt);
	simulatedProxy.Draw(dt);

	for (int i = 0; i < numHitboxes; ++i)
	{
		//hitboxes[i].DrawDebug();
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

void SNWorld::CheckCollisions()
{

}

void SNWorld::RestartGame()
{
	//Reset damage
	//Reset positions
	//Reset animations
	//Reset UI
}