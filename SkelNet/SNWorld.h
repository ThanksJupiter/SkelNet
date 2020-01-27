#pragma once
#include <vector>
#include "SNAutonomousProxy.h"
#include "Vector.h"
#include "SNFloor.h"
#include "SNSimulatedProxy.h"
#include "SNServer.h"
#include "SNClient.h"
#include "SNAnimation.h"
#include "SNAudioManager.h"
#include "SNTrailRenderer.h"

class SNTrail;
class SNParticleSystem;

class SNWorld
{
public:
	SNAudioManager* audioManager;

	SNAutonomousProxy autonomousProxy;
	SNSimulatedProxy simulatedProxy;

	SNServer server;
	SNClient client;

	Vector2 worldSize;

	SNFloor floors[3];

	static const int MAX_NUM_HITBOXES = 20;
	SNHitBox hitboxes[MAX_NUM_HITBOXES];
	int numHitboxes;

	SNAnimation* idleAnim;
	SNAnimation* runAnim;
	SNAnimation* spAttackAnim;
	SNAnimation* apAttackAnim;
	SNAnimation* walkAnim;
	SNAnimation* jumpAnim;
	SNAnimation* knockbackAnim;
	SNAnimation* dashDustAnim;
	SNAnimation* landingDustAnim;

	SNSprite* levelSprite;

	SNTrail* trail;
	SNParticleSystem* particleSystem;

	bool isServer;

	void Setup();
	void Update(float dt);
	void Draw(float dt);

	void SpawnAutonomousProxy(SNWorld& worldptr);
	void SpawnSimulatedProxy(SNWorld& worldptr);
	void SpawnFloor(Vector2 position, Vector2 size);

	SNHitBox* SpawnHitBox(Vector2 position, Vector2 size, Vector2 offset = { 0,0 }, bool blocking = false, bool callDelegates = false, std::function<void()> OnTriggerEnter = nullptr, std::function<void()> OnTriggerExit = nullptr);

	// Networking
	void SendPlayerData(Vector2 position, int health, bool serverAttacked, bool serverWasHit, bool clientAttacked, bool clientWasHit);

	void CheckCollisions();

	void RestartGame();
};
