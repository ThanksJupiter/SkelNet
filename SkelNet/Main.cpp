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
	}
}

void RestartGame()
{
	world.RestartGameEvent();
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
		startGameButton = canvas.CreateButton({ world.worldSize.x / 2 - 100, 400.f }, { 200,40.f }, true, StartGame);
		startGameButton->drawRect = true;
		startGameButton->hidden = true;

		startGameText = canvas.CreateText({ 0,0 }, "Start Game", 1.0f, &startGameButton->anchor);
		startGameText->drawRect = false;
		startGameText->hidden = true;
	}

	waitingForPlayersText = canvas.CreateText({ world.worldSize.x / 2 - 175 ,0 }, "Waiting For Players");

	hostButton = canvas.CreateButton({ 400.f, 375.f }, { 80.f,30.f }, true, SetupServer);
	joinButton = canvas.CreateButton({ 600.f, 375.f }, { 85.f,35.f }, true, SetupClient);
	ipTextInputButton = canvas.CreateButton({ (world.worldSize.x / 2) - 85, 50.f }, { 200.f,40.f }, true, EnableIPTextInput);
	nameTextInputButton = canvas.CreateButton({ (world.worldSize.x / 2) - 85, 100.f }, { 200.f,40.f }, true, EnableNameTextInput);

	hostButton->drawRect = true;
	joinButton->drawRect = true;
	ipTextInputButton->drawRect = true;
	nameTextInputButton->drawRect = true;

	hostText = canvas.CreateText({ 0,0 }, "Host", 1.0f, &hostButton->anchor);
	joinText = canvas.CreateText({ 0,0 }, "Join", 1.0f, &joinButton->anchor);
	ipInputField = canvas.CreateText({ 0,0 }, "127.0.0.1", 1.0f, &ipTextInputButton->anchor);
	nameInputField = canvas.CreateText({ 0,0 }, "Enter Name", 1.0f, &nameTextInputButton->anchor);

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

		*engGetInputTextPtr() = engGetInputText();
		//ipInputField->UpdateText(engGetInputText());

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

			Vector2 pos = world.mainCamera.MakePositionWithCam({ (float)-dstRect.w / 2, (float)-dstRect.h });
			dstRect.x = pos.x;
			dstRect.y = pos.y;

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

			if (world.isServer && startGameButton && startGameText)
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

		if (world.isServer)
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
			if (startCountDown <= 0.1f)
			{
				startCountDown += deltaTime;
				world.autonomousProxy.inputEnabled = false;
			}
			else
			{
				world.autonomousProxy.inputEnabled = true;
			}
		}
		else
		{
			if (engGetKeyDown(Key::S) || engGetButtonDown(GamepadButton::B))
			{
				SetupServer();
			}
			if (engGetKeyDown(Key::C))
			{
				SetupClient();
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
