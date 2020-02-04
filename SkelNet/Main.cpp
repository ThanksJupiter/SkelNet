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

#pragma region SetupUI

SNUIElement* hostButton;
SNUIElement* joinButton;
SNUIElement* restartbutton;
SNUIElement* textInputButton;

SNUIElement* waitingForPlayersText;
SNUIElement* startGameButton;
SNUIElement* startGameText;

SNUIElement* hostText;
SNUIElement* joinText;
SNUIElement* restartText;
SNUIElement* inputField;


void EnableSetupUI(bool bShouldDisplay)
{
	//button
	hostButton->isUsed = bShouldDisplay;
	joinButton->isUsed = bShouldDisplay;
	textInputButton->isUsed = bShouldDisplay;

	restartbutton->isUsed = !bShouldDisplay;

	//text
	hostText->isUsed = bShouldDisplay;
	joinText->isUsed = bShouldDisplay;
	//inputField->isUsed = false;

	restartText->isUsed = !bShouldDisplay;
}

#pragma endregion SetupUI

void SetupServer()
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

void SetupClient()
{
	SDL_StopTextInput();
	world.client.Setup("127.0.0.1");
	world.client.printErrors = false;
	world.client.printDebug = false;
	world.isServer = false;
	world.SpawnAutonomousProxy(world);
	world.SpawnSimulatedProxy(world);
	waitingForPlayer = false;

	EnableSetupUI(false);
}

void StartGame()
{
	if (!waitingForPlayer && !gameStarted)
	{
		gameStarted = true;
		waitingForPlayersText->isUsed = false;
		waiting = false;

		if (startGameButton && startGameText)
		{
			startGameButton->isUsed = false;
			startGameText->isUsed = false;
		}

		world.StartGameEvent();
	}
}

void RestartGame()
{
	world.RestartGameEvent();
}

void EnableTextInput()
{
	//when clicked input field
	engSetInputText("");
	SDL_StartTextInput();
}

void SetupMainMenuUI()
{
	canvas.Setup(world.worldSize, { 0.f, 0.f });

	waitingForPlayersText = canvas.CreateText({ 400,0 }, "Waiting For Players");

	startGameButton = canvas.CreateButton({ 50.f, 40.f }, { 200.f,30.f }, true, StartGame);
	startGameButton->drawRect = true;
	startGameText = canvas.CreateText({ 0,0 }, "Start Game", &startGameButton->anchor);

	hostButton = canvas.CreateButton({ 400.f, 375.f }, { 50.f,30.f }, true, SetupServer);
	joinButton = canvas.CreateButton({ 500.f, 375.f }, { 65.f,35.f }, true, SetupClient);
	restartbutton = canvas.CreateButton({ 50.f, 150.f }, { 50.f,30.f }, true, RestartGame);
	textInputButton = canvas.CreateButton({ 500.f, 50.f }, { 200.f,40.f }, true, EnableTextInput);

	hostText = canvas.CreateText({ 0,0 }, "Host", &hostButton->anchor);
	joinText = canvas.CreateText({ 0,0 }, "Join", &joinButton->anchor);
	restartText = canvas.CreateText({ 0,0 }, "Restart", &restartbutton->anchor);
	inputField = canvas.CreateText({ 0,0 }, engGetInputText().c_str(), &textInputButton->anchor);

	hostText->drawRect = true;
	joinText->drawRect = true;
	restartText->drawRect = true;
	//inputField->drawRect = true;
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

		//inputField->UpdateText(engGetInputText());

		if (engGetKeyDown(Key::Return))
		{
			SDL_StopTextInput();
		}

		if (engGetKeyDown(Key::D))
		{
			canvas.drawDebug = !canvas.drawDebug;
		}

#pragma region StartScreen

		SDL_Rect dstRect;
		dstRect.w = world.skelNetSprite->width * 2;
		dstRect.h = world.skelNetSprite->height * 2;

		Vector2 pos = world.mainCamera.MakePositionWithCam({ (float)-dstRect.w / 2, (float)-dstRect.h });
		dstRect.x = pos.x;
		dstRect.y = pos.y;

		engDrawSprite(world.skelNetSprite->sheetSourceRect, dstRect, world.skelNetSprite->texture);

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

		if (!waitingForPlayer && !startGameButton && world.isServer)
		{
			startGameButton = canvas.CreateButton({ 50.f, 40.f }, { 200.f,30.f }, true, StartGame);
			startGameButton->drawRect = true;

			startGameText = canvas.CreateText({ 0,0 }, "Start Game", &startGameButton->anchor);
		}

		//when players have joined but game hasn't begun
		if (!waitingForPlayer && waiting)
		{
			waitingForPlayersText->UpdateText("Waiting to start");

			if (world.isServer && startGameButton && startGameText)
			{
				startGameButton->isUsed = true;
				startGameText->isUsed = true;
			}
			else if (startGameButton && startGameText)
			{
				startGameButton->isUsed = false;
				startGameText->isUsed = false;
			}
		}

		if (world.isServer)
		{
			world.bWaitingToStart = waiting;
		}
		else
		{
			waiting = world.bWaitingToStart;
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
