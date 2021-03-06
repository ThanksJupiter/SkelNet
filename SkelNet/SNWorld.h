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
#include "SNCanvas.h"

class SNTrail;
class SNParticleSystem;
class SNTransform;

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
	SNAnimation* jumpSquatAnim;
	SNAnimation* somersaultAnim;
	SNAnimation* vortexDustAnim;
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
	SNAnimation* smokeAnim;
	SNAnimation* dootAnim;
	SNAnimation* deathShockwave;
	SNAnimation* hitEffect01;
	SNAnimation* laughSkelAnim;

	SNAnimation* countDownAnim;

	void PlayAnimation(SNAnimation& inAnim, Vector2 inPos, float inDuration, bool inFlipped, float inScale, float inRot);

	SNSprite* levelSprite;
	SNSprite* ropeSprite;
	SNSprite* gameTextSprite;
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

	SNUIElement* rematchButton;
	SNUIElement* rematchText;
	SNUIElement* opponentWantsRematchText;
	SNUIElement* winnerText;
	SNUIElement* gameText;

	bool doStartup;
	bool isServer;

	bool waitingToStart = true;
	bool gameEnded = false;
	float slowmotionDivider = 2.f;
	float cameraTargetPosY = 300.f;
	float cameraTargetZoomWin = 4.0f;
	float cameraDefaultZoom = 2.f;
	bool cameraFollowWinner = false;
	SNTransform* winnerTransform = nullptr;

	float spawnDistanceX = 150.0f;
	float spawnDistanceY = -300.0f;
	Vector2 deathDistance = {750, 200};

	void Setup();
	void Update(float dt);
	void Draw(float dt);

	void SetupUI();
	void SetUIColors();

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
	void RespawnPlayerEvent();
	void RematchEvent();
	void LocalRematchEvent();
	void RestartGame();
	bool opponentWantsRematch = false;
	bool wantRematch = false;
	void GameEndedEvent();
};
