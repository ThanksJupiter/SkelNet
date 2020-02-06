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
#include "SNUIElement.h"

SNWorld* thisWorld;

void REMATCH()
{
	thisWorld->LocalRematchEvent();
}

void SNWorld::PlayAnimation(SNAnimation& inAnim, Vector2 inPos, float inDuration, bool inFlipped, float inScale, float inRot)
{
	particleSystem->StartParticleEffect(inPos, &inAnim, inDuration, inFlipped, inScale, inRot);
}

void SNWorld::Setup()
{
	audioManager = new SNAudioManager;
	audioManager->InitSounds();
	//audioManager->PlayMusicLoop(audioManager->midnaLament); 

	particleSystem = new SNParticleSystem();
	particleSystem->world = this;

	trail = new SNTrail();
	SNSprite* trailSprite = new SNSprite(32, 32, nullptr, 0);
	trailSprite->texture = engLoadTexture("SN_Skel_Walk-Sheet.png");
	trail->Setup({ 70,70 }, 5, 10, 10, 5, *trailSprite);
	mainCamera.transform.SetPosition({ 0, 300 });
	mainCamera.transform.SetScale(worldSize);

	client.world = this;
	server.world = this;

	// EVENTS
	eventHandler.world = this;
	eventHandler.CreateEvent(&SNWorld::StartGameEvent, START_GAME_EVENT);
	eventHandler.CreateEvent(&SNWorld::RespawnPlayerEvent, RESPAWN_PLAYER_EVENT);
	eventHandler.CreateEvent(&SNWorld::GameEndedEvent, END_GAME_EVENT);
	eventHandler.CreateEvent(&SNWorld::RematchEvent, REMATCH_EVENT);

	thisWorld = this;
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
		RespawnPlayerEvent();
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
		mainCamera.transform.SetPosition(TranslateVector(mainCamera.transform.GetPosition(), { 0, 2 }));
	}
	if (engGetKey(Key::K))
	{
		mainCamera.transform.SetPosition(TranslateVector(mainCamera.transform.GetPosition(), { 0, -2 }));
	}

	if (engGetKeyDown(Key::G))
	{
		SNStringPacket packet;
		packet.flag = STRING_FLAG;
		packet.string = (char*)"yeho";

		if (HasAuthority())
		{
			server.SendData(&packet);
		}
		else
		{
			client.SendData(&packet);
		}
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
			autonomousProxy.transform.GetPosition().y <= -(deathDistance.y + 200))
		{
			if (autonomousProxy.stateMachine->currentStateIndex != DEATH_STATE && !autonomousProxy.isInvulnerable)
			{
				SNStatePacket packet;
				packet.flag = SP_STATE_FLAG;
				packet.state = DEATH_STATE;

				server.SendData(&packet);

				autonomousProxy.EnterState(DEATH_STATE);
				PlayerDiedEvent();
				return;
			}
		}

		if (simulatedProxy.transform.GetPosition().x >= deathDistance.x ||
			simulatedProxy.transform.GetPosition().x <= -deathDistance.x ||
			simulatedProxy.transform.GetPosition().y >= deathDistance.y ||
			simulatedProxy.transform.GetPosition().y <= -(deathDistance.y + 200))
		{
			if (simulatedProxy.stateMachine->currentStateIndex != DEATH_STATE && !simulatedProxy.isInvulnerable)
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

	if (particleSystem)
	{
		particleSystem->UpdateParticles(dt, &mainCamera);
	}

	worldFloor.Draw();

	trail->Draw(&mainCamera);

	/* Draw Health Frames */
	//worldCanvas.drawDebug = true;
	worldCanvas.Draw();
	worldCanvas.CheckInteraction();


	/* DEBUG */
	for (int i = 0; i < numHitboxes; ++i)
	{
		//hitboxes[i].DrawDebug(&mainCamera);
	}

	if (engGetKey(Key::D))
	{
		autoProxyHealthFrame->DrawDebug();
		simProxyHealthFrame->DrawDebug();

		autoProxyHealthFrame->anchor.DrawDebug(true);
		simProxyHealthFrame->anchor.DrawDebug(true);
	}

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

void SNWorld::SetupUI()
{
	worldCanvas.Setup(worldSize, { 0, 0 });

	rematchButton = worldCanvas.CreateButton({ worldSize.x / 2 - 100, (worldSize.y / 2) + 300 }, { 150,40.f }, true, REMATCH);
	rematchText = worldCanvas.CreateText({ 0,0 }, "Rematch", 1.0f, &rematchButton->anchor);
	
	opponentWantsRematchText = worldCanvas.CreateText({ 0, 50 }, "Opponent Wants Rematch!", 1.0f, &rematchButton->anchor);
	opponentWantsRematchText->SetRelativePosition({ -opponentWantsRematchText->size.x / 2, 0 });
	opponentWantsRematchText->hidden = true;

	rematchButton->drawRect = true;
	rematchButton->hidden = true;
	rematchText->hidden = true;

	winnerText = worldCanvas.CreateText({ worldSize.x / 2 , worldSize.y / 2 + 50.f }, "wins!", 3.0f);
	winnerText->SetRelativePosition({ -winnerText->size.x / 2, 0 });
	winnerText->hidden = true;

	autoProxyHealthFrame = worldCanvas.CreateRect({ 0, 0 }, { 200, 100 });
	simProxyHealthFrame = worldCanvas.CreateRect({ 0, 0 }, { 200, 100 });

	autoProxyHealthText = worldCanvas.CreateText({ 200.f, 0.f }, "0%", 2.0f, &autoProxyHealthFrame->anchor);
	autoProxyHealthText->SetRelativePosition({ -autoProxyHealthText->size.x, 0 });
	autoProxyStockText = worldCanvas.CreateText({ 0, 70.f }, "3", 1.0f, &autoProxyHealthFrame->anchor);
	autoProxyNameText = worldCanvas.CreateText({ 70.f, autoProxyHealthFrame->size.y - 50.f }, "name", 1.0f, &autoProxyHealthFrame->anchor);
	autoProxyPortrait = worldCanvas.CreateImage({ 0, 0 }, { 70.f, 70.f }, laughSkelAnim->sprites[1], &autoProxyHealthFrame->anchor);
	autoProxyPortrait->world = this;

	simProxyHealthText = worldCanvas.CreateText({ 200.f, 0.f }, "0%", 2.0f, &simProxyHealthFrame->anchor);
	simProxyHealthText->SetRelativePosition({ -simProxyHealthText->size.x, 0 });
	simProxyStockText = worldCanvas.CreateText({ 0, 70.f }, "3", 1.0f, &simProxyHealthFrame->anchor);
	simProxyNameText = worldCanvas.CreateText({ 70.f, simProxyHealthFrame->size.y - 50.f }, "name", 1.0f, &simProxyHealthFrame->anchor);
	simProxyPortrait = worldCanvas.CreateImage({ 0, 0 }, { 70.f, 70.f }, laughSkelAnim->sprites[0], &simProxyHealthFrame->anchor);
	simProxyPortrait->world = this;

	
}

void SNWorld::SetUIColors()
{

	if (HasAuthority())
	{
		autoProxyHealthFrame->SetAnchorPosition({ 100.f, worldSize.y - 100.f });
		simProxyHealthFrame->SetAnchorPosition({ worldSize.x - 300.f, worldSize.y - 100.f });

		autoProxyHealthFrame->anchor.UpdatePosition();
		simProxyHealthFrame->anchor.UpdatePosition();
		autoProxyHealthFrame->UpdatePosition();
		simProxyHealthFrame->UpdatePosition();

		simProxyHealthText->UpdatePosition();
		simProxyStockText->UpdatePosition();
		simProxyNameText->UpdatePosition();
		simProxyPortrait->UpdatePosition();

		autoProxyHealthText->UpdatePosition();
		autoProxyStockText->UpdatePosition();
		autoProxyNameText->UpdatePosition();
		autoProxyPortrait->UpdatePosition();

		autoProxyPortrait->tintR = 155;
		autoProxyPortrait->tintG = 155;
		autoProxyPortrait->tintB = 255;

		simProxyPortrait->tintR = 255;
		simProxyPortrait->tintG = 155;
		simProxyPortrait->tintB = 155;
	}
	else
	{
		autoProxyHealthFrame->SetAnchorPosition({ worldSize.x - 300.f, worldSize.y - 100.f });
		simProxyHealthFrame->SetAnchorPosition({ 100.f, worldSize.y - 100.f });

		autoProxyHealthFrame->anchor.UpdatePosition();
		simProxyHealthFrame->anchor.UpdatePosition();
		autoProxyHealthFrame->UpdatePosition();
		simProxyHealthFrame->UpdatePosition();

		simProxyHealthText->UpdatePosition();
		simProxyStockText->UpdatePosition();
		simProxyNameText->UpdatePosition();
		simProxyPortrait->UpdatePosition();

		autoProxyHealthText->UpdatePosition();
		autoProxyStockText->UpdatePosition();
		autoProxyNameText->UpdatePosition();
		autoProxyPortrait->UpdatePosition();

		autoProxyPortrait->tintR = 255;
		autoProxyPortrait->tintG = 155;
		autoProxyPortrait->tintB = 155;

		simProxyPortrait->tintR = 155;
		simProxyPortrait->tintG = 155;
		simProxyPortrait->tintB = 255;
	}
}

void SNWorld::SpawnAutonomousProxy(SNWorld& worldptr)
{
	Vector2 initPos;
	initPos.x = 0;
	initPos.y = -50.f;

	autonomousProxy = SNAutonomousProxy();
	autonomousProxy.Spawn(initPos, worldptr);

	if (HasAuthority())
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
	waitingToStart = false;

	// Do Start Game Stuff
	printf("Game Started!\n");
}

void SNWorld::RespawnPlayerEvent()
{
	if (HasAuthority())
	{
		// Send Reset Game Event
		SNEventPacket eventPacket;
		eventPacket.flag = EVENT_FLAG;
		eventPacket.eventFlag = RESPAWN_PLAYER_EVENT;
		server.SendData(&eventPacket);
	}

	// Do Restart Game Stuff
	printf("Game Restarted!\n");

	if (simulatedProxy.stateMachine->currentStateIndex == DEATH_STATE)
	{
		if (simulatedProxy.currentStocks > 0)
		{
			printf("respawn simprox, remaining stocks: %i\n", simulatedProxy.currentStocks);
			simulatedProxy.isInvulnerable = true;
			simulatedProxy.Reset();
		}
		else
		{
			GameEndedEvent();

			winnerText->hidden = false;
			std::string nameWonString(autoProxyNameText->textString);
			nameWonString.append(" wins!");
			winnerText->SetRelativePosition({ -(engGetTextSize(nameWonString.c_str()).x * 3.f) / 2, 0 });
			winnerText->UpdateText(nameWonString);


			printf("autonimus proximilian wonned! :D\n");
		}
	}

	if (autonomousProxy.stateMachine->currentStateIndex == DEATH_STATE)
	{
		if (autonomousProxy.currentStocks > 0)
		{
			printf("respawn autoprox, remaining stocks: %i\n", autonomousProxy.currentStocks);
			autonomousProxy.isInvulnerable = true;
			autonomousProxy.Reset();
		}
		else
		{
			GameEndedEvent();

			winnerText->hidden = false;
			std::string nameWonString(simProxyNameText->textString);
			nameWonString.append(" wins!");
			winnerText->SetRelativePosition({ -(engGetTextSize(nameWonString.c_str()).x * 3.f) / 2, 0 });
			winnerText->UpdateText(nameWonString);

			printf("simulensis proximilian wonned! :D\n");
		}
	}
}

void SNWorld::RematchEvent()
{
	opponentWantsRematchText->hidden = false;

	printf("rematch event!\n");

	opponentWantsRematch = true;

	if (opponentWantsRematch && wantRematch)
	{
		RestartGame();
	}
}

void SNWorld::LocalRematchEvent()
{
	printf("rematch event sent!\n");

	SNEventPacket packet;
	packet.flag = EVENT_FLAG;
	packet.eventFlag = REMATCH_EVENT;
	if (HasAuthority())
	{
		server.SendData(&packet);
	}
	else
	{
		client.SendData(&packet);
	}

	wantRematch = true;

	if (opponentWantsRematch && wantRematch)
	{
		RestartGame();
	}
}

void SNWorld::RestartGame()
{
	printf("Game restarted!\n");
	autonomousProxy.Reset();
	simulatedProxy.Reset();
	autonomousProxy.currentStocks = autonomousProxy.maxStocks;
	simulatedProxy.currentStocks = simulatedProxy.maxStocks;
	autonomousProxy.health = 1;
	simulatedProxy.health = 1;

	opponentWantsRematch = false;
	wantRematch = false;
	
	autoProxyHealthText->UpdateText("0%");
	simProxyHealthText->UpdateText("0%");

	autoProxyStockText->UpdateText(autonomousProxy.maxStocks);
	simProxyStockText->UpdateText(simulatedProxy.maxStocks);


	rematchButton->hidden = true;
	rematchText->hidden = true;
	opponentWantsRematchText->hidden = true;
	winnerText->hidden = true;
}

void SNWorld::GameEndedEvent()
{
	// TODO: Display end screen, victory name and rematch button
	if (HasAuthority())
	{
		SNEventPacket eventPacket;
		eventPacket.flag = EVENT_FLAG;
		eventPacket.eventFlag = END_GAME_EVENT;
		server.SendData(&eventPacket);
	}

	printf("Game Ended!\n");

	rematchButton->hidden = false;
	rematchText->hidden = false;
}
