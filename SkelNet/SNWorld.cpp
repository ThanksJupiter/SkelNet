#include "SNWorld.h"
#include "SNEngine.h"
#include "DataPacket.h"
#include <math.h>
#include <vector>
#include "SNAnimator.h"

void SNWorld::Update()
{
	player.Update();

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
	}
	else
	{
		Vector2 newPosition = Vector2(client.recievedData.posX, client.recievedData.posY);
		autonomousProxy.SetPosition(newPosition);
	}
}

void SNWorld::Draw(float dt)
{
	player.Draw(dt);
	autonomousProxy.Draw(dt);

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

void SNWorld::SendTransform(Vector2 position)
{
	if (!isServer)
	{
		client.transformPack.posX = position.x;
		client.transformPack.posY = position.y;
		client.transformPack.id = 0;
		client.transformPack.health = 50;

		client.SendData();
	}
	else
	{
		server.transformPack.posX = position.x;
		server.transformPack.posY = position.y;
		server.transformPack.id = 1;
		server.transformPack.health = 50;

		server.SendData();
	}
}

void SNWorld::CheckCollisions()
{

}
