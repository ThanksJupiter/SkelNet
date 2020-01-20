#pragma once
#include <vector>
#include "SNAutonomousProxy.h"
#include "Vector.h"
#include "SNFloor.h"
#include "SNSimulatedProxy.h"
#include "SNServer.h"
#include "SNClient.h"
#include "SNAnimation.h"

class SNWorld
{
public:
	SNAutonomousProxy player;
	SNSimulatedProxy autonomousProxy;

	SNServer server;
	SNClient client;

	Vector2 worldSize;
	
	SNFloor floors[3];

	SNAnimation* idleAnim;
	SNAnimation* runAnim;
	SNAnimation* attackAnim;

	bool isServer;

	void Update();
	void Draw(float dt);

	void SpawnPlayer(SNWorld& worldptr);
	void SpawnAutonomousProxy(SNWorld& worldptr);
	void SpawnFloor(Vector2 position, Vector2 size);


	// Networking
	void SendTransform(Vector2 position);


	void CheckCollisions();
	
};
