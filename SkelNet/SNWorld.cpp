#include "SNWorld.h"
#include "SNEngine.h"
#include "SNDataPacket.h"
#include <math.h>
#include <vector>
#include "SNAnimator.h"
#include "SNFlags.h"
#include "SNTrailRenderer.h"
#include "SNParticleSystem.h"
#include "SNMath.h"
#include "SNFloor.h"
#include "SNFiniteStateMachine.h"

void SNWorld::Setup()
{
	audioManager = new SNAudioManager;
	audioManager->InitSounds();
	//audioManager->PlayMusicLoop(audioManager->midnaLament);

	particleSystem = new SNParticleSystem();

	trail = new SNTrail();
	SNSprite* trailSprite = new SNSprite(32, 32, nullptr, 0);
	trailSprite->texture = engLoadTexture("SN_Skel_Walk-Sheet.png");
	trail->Setup({ 70,70 }, 5, 10, 10, 5, *trailSprite);
	mainCamera.transform.SetPosition({ 0, 100 });
	mainCamera.transform.SetScale(worldSize);

	client.world = this;
	server.world = this;

	// EVENTS
	eventHandler.world = this;
	eventHandler.CreateEvent(&SNWorld::StartGameEvent, START_GAME_EVENT);
	eventHandler.CreateEvent(&SNWorld::RestartGameEvent, RESTART_GAME_EVENT);
	eventHandler.CreateEvent(&SNWorld::GameEndedEvent, END_GAME_EVENT);
}

void SNWorld::Update(float dt)
{
	autonomousProxy.Update(dt);
	simulatedProxy.Update(dt);

	if (respawnTimerActive)
	{
		respawnTimer += dt;
	}

	if (respawnTimer >= respawnDelay)
	{
		respawnTimerActive = false;
		respawnTimer = 0.0f;
		RestartGameEvent();
	}

	Vector2 avgVector;
	avgVector = simulatedProxy.transform.GetPosition() + autonomousProxy.transform.GetPosition();
	avgVector = -avgVector * 0.5f;
	avgVector.y = mainCamera.transform.GetPosition().y;

	if (avgVector.x + (mainCamera.transform.GetScale().x / 2) < deathDistance.x &&
		avgVector.x - (mainCamera.transform.GetScale().x / 2) > -deathDistance.x)
	{
		mainCamera.transform.SetPosition(avgVector);
	}

	if (engGetKey(Key::I))
	{
		mainCamera.transform.SetPosition(TranslateVector(mainCamera.transform.GetPosition(), { 0, 1 }));
	}
	if (engGetKey(Key::K))
	{
		mainCamera.transform.SetPosition(TranslateVector(mainCamera.transform.GetPosition(), { 0, -1 }));
	}

	if (engGetKeyDown(Key::U))
	{
		mainCamera.camScale += .2f;
	}
	if (engGetKeyDown(Key::O))
	{
		mainCamera.camScale -= .2f;
	}

	if (engGetKeyDown(Key::G))
	{
		StartGameEvent();
	}

	if (engGetKeyDown(Key::H))
	{
		GameEndedEvent();
	}

	for (int i = 0; i < numHitboxes; ++i)
	{
		hitboxes[i].lastState = hitboxes[i].currentState;
		hitboxes[i].currentState.isTriggered = false;
		hitboxes[i].currentState.frameNum = engGetFrameNum();
	}

	if (HasAuthority())
	{
		simulatedProxy.attackBoxL->CheckCollision(*autonomousProxy.hitBox);
		simulatedProxy.attackBoxR->CheckCollision(*autonomousProxy.hitBox);

		autonomousProxy.attackBoxL->CheckCollision(*simulatedProxy.hitBox);
		autonomousProxy.attackBoxR->CheckCollision(*simulatedProxy.hitBox);
	}

	if (HasAuthority())
	{
		server.RecvData();
	}
	else
	{
		client.RecvData();
	}

	if (HasAuthority())
	{
		if (autonomousProxy.transform.GetPosition().x >= deathDistance.x ||
			autonomousProxy.transform.GetPosition().x <= -deathDistance.x ||
			autonomousProxy.transform.GetPosition().y >= deathDistance.y ||
			autonomousProxy.transform.GetPosition().y <= -deathDistance.y)
		{
			if (autonomousProxy.stateMachine->currentStateIndex != DEATH_STATE)
			{
				SNStatePacket packet;
				packet.flag = SP_STATE_FLAG;
				packet.state = DEATH_STATE;

				server.SendData(&packet);

				autonomousProxy.SetState(DEATH_STATE);
				PlayerDiedEvent();
				return;
			}
		}

		if (simulatedProxy.transform.GetPosition().x >= deathDistance.x ||
			simulatedProxy.transform.GetPosition().x <= -deathDistance.x ||
			simulatedProxy.transform.GetPosition().y >= deathDistance.y ||
			simulatedProxy.transform.GetPosition().y <= -deathDistance.y)
		{
			if (simulatedProxy.stateMachine->currentStateIndex != DEATH_STATE)
			{
				SNStatePacket packet;
				packet.flag = AP_STATE_FLAG;
				packet.state = DEATH_STATE;

				server.SendData(&packet);

				simulatedProxy.SetState(DEATH_STATE);
				PlayerDiedEvent();
				return;
			}
		}
	}
}

void SNWorld::Draw(float dt)
{
	engSetSpriteRenderScale(mainCamera.camScale);

	simulatedProxy.Draw(dt, &mainCamera);
	autonomousProxy.Draw(dt, &mainCamera);

	for (int i = 0; i < numHitboxes; ++i)
	{
		//hitboxes[i].DrawDebug(&mainCamera);
	}

	//floors[0].Draw();

	if (particleSystem)
	{
		particleSystem->UpdateParticles(dt, &mainCamera);
	}

	worldFloor.Draw();

	trail->Draw(&mainCamera);

	if (engGetKey(Key::R))

	{
		engSetColor(255, 0, 0);

		Vector2 camCenter = mainCamera.GetCenterPosition();
		Vector2 topRight = { deathDistance.x, -deathDistance.y };
		Vector2 topLeft = -deathDistance;
		Vector2 bottomRight = deathDistance;
		Vector2 bottomLeft = { -deathDistance.x, deathDistance.y };

		engDrawPoint(mainCamera.MakePositionWithCam(topRight), 20);
		engDrawPoint(mainCamera.MakePositionWithCam(topLeft), 20);
		engDrawPoint(mainCamera.MakePositionWithCam(bottomRight), 20);
		engDrawPoint(mainCamera.MakePositionWithCam(bottomLeft), 20);

		engDrawLine(mainCamera.MakePositionWithCam(topLeft), mainCamera.MakePositionWithCam(topRight));
		engDrawLine(mainCamera.MakePositionWithCam(topLeft), mainCamera.MakePositionWithCam(bottomLeft));
		engDrawLine(mainCamera.MakePositionWithCam(bottomRight), mainCamera.MakePositionWithCam(topRight));
		engDrawLine(mainCamera.MakePositionWithCam(bottomRight), mainCamera.MakePositionWithCam(bottomLeft));
		engSetColor(0, 0, 0);
	}
}

void SNWorld::SpawnAutonomousProxy(SNWorld& worldptr)
{
	Vector2 initPos;
	initPos.x = 0;
	initPos.y = -50.f;

	autonomousProxy = SNAutonomousProxy();
	autonomousProxy.Spawn(initPos, worldptr);

	if (isServer)
	{
		autonomousProxy.animator->r = 155;
		autonomousProxy.animator->g = 155;
		autonomousProxy.animator->b = 255;
	}
	else
	{
		autonomousProxy.animator->r = 255;
		autonomousProxy.animator->g = 155;
		autonomousProxy.animator->b = 155;
	}
}

void SNWorld::SpawnSimulatedProxy(SNWorld& worldptr)
{
	Vector2 initPos;
	initPos.x = 0;
	initPos.y = -10;

	simulatedProxy.Spawn(initPos, worldptr);

	if (isServer)
	{
		simulatedProxy.animator->r = 255;
		simulatedProxy.animator->g = 155;
		simulatedProxy.animator->b = 155;
	}
	else
	{
		simulatedProxy.animator->r = 155;
		simulatedProxy.animator->g = 155;
		simulatedProxy.animator->b = 255;
	}
}

void SNWorld::SpawnFloor(Vector2 position, Vector2 size)
{
	worldFloor.Spawn(position, size, this);
}

SNHitBox* SNWorld::SpawnHitBox(Vector2 position, Vector2 size, Vector2 offset, char id, bool blocking, bool callDelegates, std::function<void()> OnTriggerEnter, std::function<void()> OnTriggerExit)
{
	hitboxes[numHitboxes].Setup(position, size, offset, id, blocking, callDelegates, OnTriggerEnter, OnTriggerExit);
	hitboxes[numHitboxes].drawDebug = true;

	numHitboxes++;

	return &hitboxes[numHitboxes - 1];
}

bool SNWorld::HasAuthority()
{
	return isServer;
}

void SNWorld::PlayerDiedEvent()
{
	if (HasAuthority())
	{
		// restart timer
		respawnTimerActive = true;
	}

	// Do pLAYEr dieD Stuff
	printf("Player died!\n");
}

void SNWorld::StartGameEvent()
{
	if (HasAuthority())
	{
		// Send Start Game Event
		SNEventPacket eventPacket;
		eventPacket.flag = EVENT_FLAG;
		eventPacket.eventFlag = START_GAME_EVENT;
		server.SendData(&eventPacket);
	}
	else
	{
		bWaitingToStart = false;
	}

	// Do Start Game Stuff
	printf("Game Started!\n");
}

void SNWorld::RestartGameEvent()
{
	if (HasAuthority())
	{
		// Send Reset Game Event
		SNEventPacket eventPacket;
		eventPacket.flag = EVENT_FLAG;
		eventPacket.eventFlag = RESTART_GAME_EVENT;
		server.SendData(&eventPacket);
	}

	// Do Restart Game Stuff
	printf("Game Restarted!\n");

	if (simulatedProxy.stateMachine->currentStateIndex == DEATH_STATE)
	{
		if (simulatedProxy.currentStocks > 0)
		{	
			printf("respawn simprox, remaining stocks: %i\n", simulatedProxy.currentStocks);
			simulatedProxy.Reset();
		}
		else
		{
			printf("autonimus proximilian wonned! :D\n");
		}
	}

	if (autonomousProxy.stateMachine->currentStateIndex == DEATH_STATE)
	{
		if (autonomousProxy.currentStocks > 0)
		{
			printf("respawn autoprox, remaining stocks: %i\n", autonomousProxy.currentStocks);
			autonomousProxy.Reset();
		}
		else
		{
			printf("simulensis proximilian wonned! :D\n");
		}
	}
}

void SNWorld::GameEndedEvent()
{
	if (HasAuthority())
	{
		// Send End Game Event
		SNEventPacket eventPacket;
		eventPacket.flag = EVENT_FLAG;
		eventPacket.eventFlag = END_GAME_EVENT;
		server.SendData(&eventPacket);
	}

	// Do End Game Stuff
	printf("Game Ended!\n");
}
