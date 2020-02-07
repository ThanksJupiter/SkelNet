#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>

#include "SNEngine.h"
#include "Vector.h"
#include "SNWorld.h"
#include "SNServer.h"
#include "SNClient.h"
#include "Key.h"
#include "SNUIElement.h"
#include "SNCanvas.h"
#include "SpritesheetData.h"
#include "SNSprite.h"
#include "SNAnimator.h"
#include "SNAutonomousProxy.h"
#include "SNParticleSystem.h"

SNWorld world;
bool waiting = true;
//todo:: if debugging; initialize bool as false to start without client
bool waitingForPlayer = true;
bool gameStarted = false;
SNCanvas canvas;

float startCountDown;
bool chainSoundStarted = false;

#pragma region SetupUI

SNUIElement* hostButton;
SNUIElement* joinButton;
SNUIElement* ipTextInputButton;
SNUIElement* nameTextInputButton;

SNUIElement* waitingForPlayersText;
SNUIElement* startGameButton;
SNUIElement* startGameText;

SNUIElement* hostText;
SNUIElement* joinText;
SNUIElement* ipInputField;
SNUIElement* nameInputField;

void EnableSetupUI(bool shouldDisplay)
{
	//button
	hostButton->isUsed = shouldDisplay;
	joinButton->isUsed = shouldDisplay;
	ipTextInputButton->isUsed = shouldDisplay;
	nameTextInputButton->isUsed = shouldDisplay;

	//text
	hostText->isUsed = shouldDisplay;
	joinText->isUsed = shouldDisplay;
	ipInputField->isUsed = false;
	nameInputField->isUsed = false;
}

#pragma endregion SetupUI

void SetupServer()
{
	if (!world.server.allreadySetUp)
	{
		SDL_StopTextInput();
		world.server.Setup();
		world.server.printErrors = false;
		world.server.printDebug = false;
		world.isServer = true;
		world.SpawnAutonomousProxy(world);
		world.SpawnSimulatedProxy(world);
		EnableSetupUI(false);
	}
}

void SetupClient()
{
	if (world.client.Setup(ipInputField->textString.c_str()))
	{
		SDL_StopTextInput();
		world.client.printErrors = false;
		world.client.printDebug = false;
		world.isServer = false;
		world.SpawnAutonomousProxy(world);
		world.SpawnSimulatedProxy(world);
		waitingForPlayer = false;

		EnableSetupUI(false);
	}
}


void StartGame()
{
	if (!waitingForPlayer && !gameStarted)
	{
		world.particleSystem->StartParticleEffect({ 0, 0 }, world.countDownAnim, world.countDownAnim->duration, false, 6);

		gameStarted = true;
		waitingForPlayersText->isUsed = false;
		waiting = false;

		//remove start button
		if (startGameButton && startGameText)
		{
			startGameButton->isUsed = false;
			startGameText->isUsed = false;
		}

		if (world.HasAuthority())
		{
			world.StartGameEvent();
		}


		world.autoProxyNameText->UpdateText(nameInputField->textString);
		SNStringPacket packet;
		packet.flag = STRING_FLAG;
		packet.string = (char*)nameInputField->textString.c_str();
		if (world.HasAuthority())
		{
			world.server.SendData(&packet);
		}
		else
		{
			world.client.SendData(&packet);
		}

		world.SetUIColors();
	}
}

void RestartGame()
{
	world.RespawnPlayerEvent();
}

void EnableIPTextInput()
{
	//when clicked input field
	engSetInputTextPtr(&ipInputField->textString);
	*engGetInputTextPtr() = "";
	SDL_StartTextInput();
}

void EnableNameTextInput()
{
	//when clicked input field
	engSetInputTextPtr(&nameInputField->textString);
	*engGetInputTextPtr() = "";
	SDL_StartTextInput();
}

void SetupMainMenuUI()
{
	canvas.Setup(world.worldSize, { 0.f, 0.f });

	//Start game butt on
	{
		startGameButton = canvas.CreateButton({ world.worldSize.x / 2 - 100, (world.worldSize.y / 2) + 250 }, { 250,50.f }, true, StartGame);
		startGameButton->drawRect = true;
		startGameButton->hidden = true;

		startGameText = canvas.CreateText({ 25, 10 }, "Start Game", 1.0f, &startGameButton->anchor);
		startGameText->drawRect = false;
		startGameText->hidden = true;
	}

	waitingForPlayersText = canvas.CreateText({ world.worldSize.x / 2 - 175 ,0 }, "Waiting For Players");

	hostButton = canvas.CreateButton({ (world.worldSize.x / 2) - 190, (world.worldSize.y / 2) + 350 }, { 135.f,60.f }, true, SetupServer);
	joinButton = canvas.CreateButton({ (world.worldSize.x / 2) + 125, (world.worldSize.y / 2) + 350 }, { 135.f,60.f }, true, SetupClient);
	
	ipTextInputButton = canvas.CreateButton({ (world.worldSize.x / 2) - 85, (world.worldSize.y / 2) + 150 }, { 250.f,50.f }, true, EnableIPTextInput);
	nameTextInputButton = canvas.CreateButton({ (world.worldSize.x / 2) - 85, (world.worldSize.y / 2) + 220 }, { 250.f,50.f }, true, EnableNameTextInput);

	hostButton->drawRect = true;
	joinButton->drawRect = true;
	ipTextInputButton->drawRect = true;
	nameTextInputButton->drawRect = true;

	hostText = canvas.CreateText({ 26, 15 }, "Host", 1.0f, &hostButton->anchor);
	joinText = canvas.CreateText({ 24, 15 }, "Join", 1.0f, &joinButton->anchor);
	ipInputField = canvas.CreateText({ 31,10 }, "127.0.0.1", 1.0f, &ipTextInputButton->anchor);
	nameInputField = canvas.CreateText({ 24,10 }, "Enter Name", 1.0f, &nameTextInputButton->anchor);

	engSetInputTextPtr(&ipInputField->textString);

	hostText->drawRect = false;
	joinText->drawRect = false;
	ipInputField->drawRect = false;
	nameInputField->drawRect = false;

}

int main()
{
	srand(time(nullptr));

	engInit();

	engLoadAnimationsToWorld(world);

	world.worldSize = { (float)engGetWidth(), (float)engGetHeight() };
	world.Setup();
	world.SetupUI();

	world.SpawnFloor({ 0, 0 }, { 3, 3 });

	SetupMainMenuUI();

	SDL_StopTextInput();

	// delta time
	uint64_t NOW = SDL_GetPerformanceCounter();
	uint64_t LAST = 0;
	double deltaTime = 0;
	float timer = 0;

	while (!engShouldQuit())
	{
		canvas.CheckInteraction();
		canvas.Draw();

		engRender();
		engUpdate();

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = ((double)((NOW - LAST) * 1000) / (double)SDL_GetPerformanceFrequency()) * 0.001;

		if (world.isServer)
		{
			engDrawString({ 10, 10 }, "Server");
			if (world.server.AcceptConnection())
			{
				waitingForPlayer = false;
			}
		}
		else
		{
			engDrawString({ 10, 10 }, "Client");
		}

		if (engGetInputText().size() > 10)
		{
			engGetInputTextPtr()->pop_back();
			printf(engGetInputText().c_str());
		}

		if (engGetKeyDown(Key::Return))
		{
			SDL_StopTextInput();
		}

		if (engGetKeyDown(Key::D))
		{
			canvas.drawDebug = !canvas.drawDebug;
		}

#pragma region StartScreen

		if (!gameStarted)
		{
			SDL_Rect dstRect;
			dstRect.w = world.skelNetSprite->width * 2;
			dstRect.h = world.skelNetSprite->height * 2;

			dstRect.x = ((engGetWidth() / 2) - dstRect.w) / engGetRenderScale();
			dstRect.y = ((engGetHeight() / 2) - dstRect.h) / engGetRenderScale();

			engDrawSprite(world.skelNetSprite->sheetSourceRect, dstRect, world.skelNetSprite->texture);
		}

		//recieve data to know if game has started
		if (world.HasAuthority())
		{
			world.server.RecvData();
		}
		else
		{
			world.client.RecvData();
		}

		//before players have joined
		if (waitingForPlayer && !waiting)
		{
			waitingForPlayersText->isUsed = true;
			waitingForPlayersText->UpdateText("Waiting For Players");
		}

		//when players have joined but game hasn't begun
		if (!waitingForPlayer && waiting)
		{
			waitingForPlayersText->UpdateText("Waiting to start");

			if (world.HasAuthority() && startGameButton && startGameText)
			{
				startGameButton->hidden = false;
				startGameText->hidden = false;
			}
			else if (startGameButton && startGameText)
			{
				startGameButton->hidden = true;
				startGameText->hidden = true;
			}
		}

		if (world.HasAuthority())
		{
			world.waitingToStart = waiting;
		}
		else
		{
			waiting = world.waitingToStart;
			if (!waiting && !waitingForPlayer && !gameStarted)
			{
				StartGame();
			}
		}

#pragma endregion StartScreen

		if (!waiting && !waitingForPlayer)
		{
			waitingForPlayersText->isUsed = false;

			world.Update(deltaTime);
			world.Draw(deltaTime);
			canvas.CheckInteraction();
			canvas.Draw();

			//countdown to start game
			if (startCountDown <= 3.0f)
			{
				float currentCamY = world.mainCamera.transform.GetPosition().y;
				float stepDistance = currentCamY + deltaTime * (world.cameraTargetPosY - currentCamY);
				world.mainCamera.transform.SetPosition({ world.mainCamera.transform.GetPosition().x , stepDistance });

				startCountDown += deltaTime;
				world.autonomousProxy.inputEnabled = false;
				if (!chainSoundStarted)
				{
					chainSoundStarted = true;
					world.audioManager->LoopChunk(world.audioManager->chainSound, 2);
				}
			}
			else if (chainSoundStarted == true)
			{
				chainSoundStarted = false;
				world.audioManager->StopChannel(2);
				world.audioManager->PlayChunkOnce(world.audioManager->platformLand);
				world.autonomousProxy.inputEnabled = true;
			}
		}
		else
		{
			if (engGetKeyDown(Key::S) || engGetButtonDown(GamepadButton::B))
			{
				//SetupServer();
			}
			if (engGetKeyDown(Key::C))
			{
				//SetupClient();
			}
			SDL_Delay(.5f);
		}
	}

	if (world.isServer)
	{
		world.server.Close();
	}
	else
	{
		world.client.Close();
	}

	engClose();
	return 0;
}
