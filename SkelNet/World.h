#pragma once
#include <vector>
#include "AutonomousProxy.h"
#include "Vector.h"
#include "Floor.h"
#include "SimulatedProxy.h"
#include "Server.h"
#include "Client.h"

class World
{
public:
	AutonomousProxy player;
	SimulatedProxy autonomousProxy;

	Server server;
	Client client;

	Vector2 worldSize;
	
	Floor floors[3];

	bool isServer;

	void Update();
	void Draw();

	void SpawnPlayer(World& worldptr);
	void SpawnAutonomousProxy();
	void SpawnFloor(Vector2 position, Vector2 size);


	// Networking
	void SendTransform(Vector2 position);


	void CheckCollisions();
	
};
