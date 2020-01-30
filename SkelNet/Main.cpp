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
	world.server.Setup();
	world.server.printDebug = false;
	world.isServer = true;
	world.SpawnAutonomousProxy(world);
	world.SpawnSimulatedProxy(world);
	waiting = false;

}

//todo: client freezes on startup
void SetupClient()
{
	world.client.Setup();
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

int main()
{
	srand(time(nullptr));

	engInit();

	engLoadAnimationsToWorld(world);

	world.worldSize = { (float)engGetWidth(), (float)engGetHeight() };
	world.Setup();

	world.SpawnFloor({ 0, 0 }, { 1, 1 });


	canvas.Setup(world.worldSize, { 0.f, 0.f });
	SNUIElement* rect = canvas.CreateRect({ 30.f, 30.f }, { 40.f,20.f });
	SNUIElement* hostButton = canvas.CreateButton({ 50.f, 40.f }, { 50.f,30.f }, true, SetupServer);
	SNUIElement* joinButton = canvas.CreateButton({ 50.f, 100.f }, { 50.f,30.f }, true, SetupClient);
	SNUIElement* restartbutton = canvas.CreateButton({ 50.f, 160.f }, { 50.f,30.f }, true, RestartGame);
	canvas.CreateText({ 0,0 }, "Host", &hostButton->anchor);
	canvas.CreateText({ 0,0 }, "Join", &joinButton->anchor);
	canvas.CreateText({ 0,0 }, "Restart", &restartbutton->anchor);

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

			if (engGetKeyDown(Key::D))
			{
				canvas.drawDebug = !canvas.drawDebug;
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
