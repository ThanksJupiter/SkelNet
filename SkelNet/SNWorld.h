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

	SNHitBox hitbox[20];
	int numHitboxes;

	SNAnimation* idleAnim;
	SNAnimation* runAnim;
	SNAnimation* attackAnim;

	bool isServer;

	void Update();
	void Draw(float dt);

	void SpawnPlayer(SNWorld& worldptr);
	void SpawnAutonomousProxy(SNWorld& worldptr);
	void SpawnFloor(Vector2 position, Vector2 size);

	void SpawnHitBox(Vector2 position, Vector2 size, Vector2 offset = { 0,0 }, bool blocking = false, bool callDelegates = false, std::function<void()> OnTriggerEnter = nullptr, std::function<void()> OnTriggerExit = nullptr);

	// Networking
	void SendPlayerData(Vector2 position, int health);


	void CheckCollisions();

};