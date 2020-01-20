#pragma once
#include <vector>
#include "SNAutonomousProxy.h"
#include "Vector.h"
#include "SNFloor.h"
#include "SNSimulatedProxy.h"
#include "SNServer.h"
#include "SNClient.h"

class SNWorld
{
public:
	SNAutonomousProxy player;
	SNSimulatedProxy autonomousProxy;

	SNServer server;
	SNClient client;

	Vector2 worldSize;
	
	SNFloor floors[3];

	bool isServer;

	void Update();
	void Draw();

	void SpawnPlayer(SNWorld& worldptr);
	void SpawnAutonomousProxy();
	void SpawnFloor(Vector2 position, Vector2 size);

	// Networking
	void SendPlayerData(Vector2 position, int health);


	void CheckCollisions();
	
};
