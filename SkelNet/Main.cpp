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

void SetupServer()
{
	SDL_StopTextInput();
	world.server.Setup();
	world.server.printErrors = false;
	world.server.printDebug = false;
	world.isServer = true;
	world.SpawnAutonomousProxy(world);
	world.SpawnSimulatedProxy(world);
	waiting = false;
}

//todo: client freezes on startup
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
}

void RestartGame()
{
	world.RestartGame();
}

void EnableTextInput()
{
	// when clicked input field
	engSetInputText("");
	SDL_StartTextInput();
}

int main()
{
	srand(time(nullptr));

	engInit();

	engLoadAnimationsToWorld(world);

	world.worldSize = { (float)engGetWidth(), (float)engGetHeight() };
	world.Setup();

	world.SpawnFloor({ 0, 0 }, { 2, 2 });

	canvas.Setup(world.worldSize, { 0.f, 0.f });
	SNUIElement* rect = canvas.CreateRect({ 30.f, 30.f }, { 40.f,20.f });
	SNUIElement* hostButton = canvas.CreateButton({ 50.f, 40.f }, { 50.f,30.f }, true, SetupServer);
	SNUIElement* joinButton = canvas.CreateButton({ 50.f, 100.f }, { 50.f,30.f }, true, SetupClient);
	SNUIElement* restartbutton = canvas.CreateButton({ 50.f, 150.f }, { 50.f,30.f }, true, RestartGame);
	SNUIElement* textInputButton = canvas.CreateButton({ 500.f, 50.f },{ 200.f,40.f }, true, EnableTextInput);

	canvas.CreateText({ 0,0 }, "Host", &hostButton->anchor);
	canvas.CreateText({ 0,0 }, "Join", &joinButton->anchor);
	canvas.CreateText({ 0,0 }, "Restart", &restartbutton->anchor);
	SNUIElement* inputField = canvas.CreateText({ 0,0 }, engGetInputText().c_str(), &textInputButton->anchor);
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
