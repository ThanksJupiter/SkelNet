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

	SNHitBox hitbox[20];
	int numHitboxes;

	bool isServer;

	void Update();
	void Draw();

	void SpawnPlayer(SNWorld& worldptr);
	void SpawnAutonomousProxy();
	void SpawnFloor(Vector2 position, Vector2 size);
	void SpawnHitBox(Vector2 position, Vector2 size, Vector2 offset = { 0,0 }, bool blocking = false, bool callDelegates = false, std::function<void()> OnTriggerEnter = nullptr, std::function<void()> OnTriggerExit = nullptr);

	// Networking
	void SendTransform(Vector2 position);


	void CheckCollisions();
	
};
