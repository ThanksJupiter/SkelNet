#include "SNWorld.h"
#include "SNEngine.h"
#include "DataPacket.h"
#include <math.h>
#include <vector>
#include "SNAnimator.h"

void SNWorld::Update(float dt)
{
	player.Update(dt);

	for (int i = 0; i < numHitboxes; ++i)
	{
		hitbox[i].CheckCollision(player.hitBox);
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
		autonomousProxy.SetPosition(newPosition);
		autonomousProxy.health = server.recievedData.health;
	}
	else
	{
		Vector2 newPosition = Vector2(client.recievedData.posX, client.recievedData.posY);
		autonomousProxy.SetPosition(newPosition);
		autonomousProxy.health = client.recievedData.health;
	}
}

void SNWorld::Draw(float dt)
{
	player.Draw(dt);
	autonomousProxy.Draw(dt);

	for (int i = 0; i < numHitboxes; ++i)
	{
		hitbox[i].DrawDebug();
	}

	floors[0].Draw();
}

void SNWorld::SpawnPlayer(SNWorld& worldptr)
{
	Vector2 initPos;
	initPos.x = worldSize.x / 2;
	initPos.y = (worldSize.y / 3) * 2;

	player = SNAutonomousProxy();
	player.Spawn({ initPos.x + 50, initPos.y }, worldptr);
}

void SNWorld::SpawnAutonomousProxy(SNWorld& worldptr)
{
	Vector2 initPos;
	initPos.x = worldSize.x / 2;
	initPos.y = (worldSize.y / 3) * 2;

	autonomousProxy.Spawn(initPos, worldptr);
}

void SNWorld::SpawnFloor(Vector2 position, Vector2 size)
{
	floors[0].position = position;
	floors[0].size = size;
}

void SNWorld::SpawnHitBox(Vector2 position, Vector2 size, Vector2 offset, bool blocking, bool callDelegates, std::function<void()> OnTriggerEnter, std::function<void()> OnTriggerExit)
{
	hitbox[numHitboxes].Setup(position, size, offset, blocking, callDelegates, OnTriggerEnter, OnTriggerExit);
	hitbox[numHitboxes].drawDebug = true;

	numHitboxes++;
}

void SNWorld::SendPlayerData(Vector2 position, int health)
{
	if (!isServer)
	{
		client.transformPack.posX = position.x;
		client.transformPack.posY = position.y;
		client.transformPack.id = 0;
		client.transformPack.health = health;

		client.SendData();
	}
	else
	{
		server.transformPack.posX = position.x;
		server.transformPack.posY = position.y;
		server.transformPack.id = 1;
		server.transformPack.health = health;

		server.SendData();
	}
}

void SNWorld::CheckCollisions()
{

}
