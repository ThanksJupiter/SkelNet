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

SNWorld world;
bool waiting = true;

SNCanvas canvas;

void SetupServer()
{
	world.server.Setup();
	world.server.printDebug = true;
	world.isServer = true;
	world.SpawnPlayer(world);
	world.SpawnAutonomousProxy();

	waiting = false;
}

void SetupClient()
{
	world.client.Setup();
	world.client.printDebug = true;
	world.isServer = false;
	world.SpawnPlayer(world);
	world.SpawnAutonomousProxy();

	waiting = false;
}

void Print()
{
	printf("Clicked on the button!\n");
}

int main()
{
	srand(time(nullptr));

	engInit();

	world.worldSize = { (float)engGetWidth(), (float)engGetHeight() };
	world.SpawnFloor({ 0, (world.worldSize.y / 3) * 2 }, { world.worldSize.x, 20 });

	canvas.Setup(world.worldSize / 2.f, { 100.f, 70.f });
	SNUIElement* rect = canvas.CreateRect({ 30.f, 30.f }, { 40.f,20.f });
	SNUIElement* hostButton =  canvas.CreateButton({ 0.f, 40.f }, { 50.f,30.f }, true, SetupServer, &rect->anchor);
	SNUIElement* joinButton = canvas.CreateButton({ 0.f, 100.f }, { 50.f,30.f }, true, SetupClient, &rect->anchor);
	canvas.CreateText({ 0,0 }, "Host", &hostButton->anchor);
	canvas.CreateText({ 0,0 }, "Join", &joinButton->anchor);

	while (!engShouldQuit())
	{
		canvas.CheckInteraction();
		canvas.Draw();

		// Move further down?
		engRender();
		engUpdate();

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
			world.Update();
			world.Draw();
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
		//else
		//{
		//	if (engGetKeyDown(Key::S))
		//	{
		//		world.server.Setup();
		//		world.server.printDebug = true;
		//		world.isServer = true;
		//		world.SpawnPlayer(world);
		//		world.SpawnAutonomousProxy();
		//
		//		waiting = false;
		//	}
		//
		//	if (engGetKeyDown(Key::C))
		//	{
		//		world.client.Setup();
		//		world.client.printDebug = true;
		//		world.isServer = false;
		//		world.SpawnPlayer(world);
		//		world.SpawnAutonomousProxy();
		//
		//		waiting = false;
		//	}
		//}
		SDL_Delay(.5f);
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
