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
#include "SNCamera.h"
#include "SNEvent.h"
#include "SNEventHandler.h"

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
	SNAnimation* runDustAnim;
	SNAnimation* landingDustAnim;
	SNAnimation* coolDustAnim;
	SNAnimation* fallAnim;
	SNAnimation* knockedDownAnim;
	SNAnimation* dustCloud01Anim;
	SNAnimation* turnAroundAnim;
	SNAnimation* teabagAnim;
	SNAnimation* deathShockwave;
	SNAnimation* hitEffect01;

	SNAnimation* countDownAnim;

	void PlayAnimation(SNAnimation& inAnim, Vector2 inPos, float inDuration, bool inFlipped, float inScale, float inRot);

	SNSprite* levelSprite;
	SNSprite* skelNetSprite;
	SNFloor worldFloor;

	SNTrail* trail;
	SNParticleSystem* particleSystem;

	SNCamera mainCamera;

	SNCanvas worldCanvas;

	// Health Frames
	SNUIElement* autoProxyHealthFrame;
	SNUIElement* autoProxyHealthText;
	SNUIElement* autoProxyStockText;
	SNUIElement* autoProxyNameText;
	SNUIElement* autoProxyPortrait;

	SNUIElement* simProxyHealthFrame;
	SNUIElement* simProxyHealthText;
	SNUIElement* simProxyStockText;
	SNUIElement* simProxyNameText;
	SNUIElement* simProxyPortrait;

	bool doStartup;
	bool isServer;

	bool waitingToStart = true;

	float spawnDistanceX = 150.0f;
	float spawnDistanceY = -300.0f;
	Vector2 deathDistance = {750, 200};

	void Setup();
	void Update(float dt);
	void Draw(float dt);

	void SpawnAutonomousProxy(SNWorld& worldptr);
	void SpawnSimulatedProxy(SNWorld& worldptr);
	void SpawnFloor(Vector2 position, Vector2 size);

	SNHitBox* SpawnHitBox(Vector2 position, Vector2 size, Vector2 offset = { 0,0 }, char id = -1, bool blocking = false, bool callDelegates = false, std::function<void()> OnTriggerEnter = nullptr, std::function<void()> OnTriggerExit = nullptr);

	// NETWORKING
	bool HasAuthority();

	// EVENTS
	SNEventHandler eventHandler;
	void PlayerDiedEvent();
	// death timer thing
	float respawnTimer = 0.0f;
	float respawnDelay = 2.0f;
	bool respawnTimerActive = false;

	void StartGameEvent();
	void RestartGameEvent();
	//void RematchEvent();
	//bool opponentWantsRematch = false;
	void GameEndedEvent();
};
