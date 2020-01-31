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

SNCanvas canvas;

#pragma region SetupUI

SNUIElement* hostButton;
SNUIElement* joinButton;
SNUIElement* restartbutton;
SNUIElement* textInputButton;

SNUIElement* hostText;
SNUIElement* joinText;
SNUIElement* restartText;
SNUIElement* inputField;

void ShowSetupUI(bool bShouldDisplay)
{
	//button
	hostButton->isUsed = bShouldDisplay;
	joinButton->isUsed = bShouldDisplay;
	textInputButton->isUsed = bShouldDisplay;

	restartbutton->isUsed = !bShouldDisplay;

	//text
	hostText->isUsed = bShouldDisplay;
	joinText->isUsed = bShouldDisplay;
	inputField->isUsed = false;

	restartText->isUsed = !bShouldDisplay;
}
#pragma endregion SetupUI

void SetupServer()
{ 
	//todo (Kasper): when starting game, wait for players to join
	SDL_StopTextInput();
	world.server.Setup();
	world.server.printErrors = false;
	world.server.printDebug = false;
	world.isServer = true;
	world.SpawnAutonomousProxy(world);
	world.SpawnSimulatedProxy(world);
	waiting = false;

	ShowSetupUI(false);
}

void SetupClient()
{
	SDL_StopTextInput();
	world.client.Setup(engGetInputText().c_str());
	world.client.printErrors = false;
	world.client.printDebug = false;
	world.isServer = false;
	world.SpawnAutonomousProxy(world);
	world.SpawnSimulatedProxy(world);
	waiting = false;

	ShowSetupUI(false);
}

void RestartGame()
{
	world.RestartGame();
}

void EnableTextInput()
{
	//when clicked input field
	engSetInputText("");
	SDL_StartTextInput();
}

void EnableSetupUI()
{
	canvas.Setup(world.worldSize, { 0.f, 0.f });
	hostButton = canvas.CreateButton({ 50.f, 40.f }, { 50.f,30.f }, true, SetupServer);
	joinButton = canvas.CreateButton({ 50.f, 100.f }, { 50.f,30.f }, true, SetupClient);
	restartbutton = canvas.CreateButton({ 50.f, 150.f }, { 50.f,30.f }, true, RestartGame);
	textInputButton = canvas.CreateButton({ 500.f, 50.f }, { 200.f,40.f }, true, EnableTextInput);

	hostText = canvas.CreateText({ 0,0 }, "Host", &hostButton->anchor);
	joinText = canvas.CreateText({ 0,0 }, "Join", &joinButton->anchor);
	restartText = canvas.CreateText({ 0,0 }, "Restart", &restartbutton->anchor);
	inputField = canvas.CreateText({ 0,0 }, engGetInputText().c_str(), &textInputButton->anchor);

	hostText->drawRect = true;
	joinText->drawRect = true;
	restartText->drawRect = true;
	inputField->drawRect = true;
}

int main()
{
	srand(time(nullptr));

	engInit();

	engLoadAnimationsToWorld(world);

	world.worldSize = { (float)engGetWidth(), (float)engGetHeight() };
	world.Setup();

	world.SpawnFloor({ 0, 0 }, { 3, 3 });

	EnableSetupUI();

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
		}
		else
		{
			engDrawString({ 10, 10 }, "Client");
		}

		inputField->UpdateText(engGetInputText());

		if (engGetKeyDown(Key::Return))
		{
			SDL_StopTextInput();
		}

		if (engGetKeyDown(Key::D))
		{
			canvas.drawDebug = !canvas.drawDebug;
		}

		if (!waiting)
		{
			world.Update(deltaTime);
			world.Draw(deltaTime);
			canvas.CheckInteraction();
			canvas.Draw();

			if (engGetKeyDown(Key::A) && world.isServer == true)
			{
				world.server.AcceptConnection();
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
