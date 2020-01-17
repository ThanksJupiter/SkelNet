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

SNWorld world;
bool waiting = true;

SNCanvas canvas;

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
	canvas.CreateButton({ 60.f, 80.f }, { 50.f,30.f }, true, Print, &rect->anchor);
	canvas.CreateText({ 60.f, 80.f }, "Heasasass", &rect->anchor, { 20.f, 60.f });

	Vector2 position = Vector2(0, 0);
	Vector2 size = Vector2(32, 32);
	SNSprite sprite = SNSprite(position, size, LoadTexture("spritesheet.png"));

	while (!engShouldQuit())
	{
		// Move further down?
		engRender();
		engUpdate();

		if (world.isServer)
		{
			engDrawString({ 10, 10 }, "SERVER");
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
		else
		{
			if (engGetKeyDown(Key::S))
			{
				world.server.Setup();
				waiting = false;

				world.isServer = true;
				world.SpawnPlayer(world);
				world.SpawnAutonomousProxy();
			}

			if (engGetKeyDown(Key::C))
			{
				world.client.Setup();
				waiting = false;

				world.isServer = false;
				world.SpawnPlayer(world);
				world.SpawnAutonomousProxy();
			}
		}
		else
		{
			if (engGetKeyDown(Key::S))
			{
				world.server.Setup();
				//world.server.printDebug = true;
				world.isServer = true;
				world.SpawnPlayer(world);
				world.SpawnAutonomousProxy();

				waiting = false;
			}

			if (engGetKeyDown(Key::C))
			{
				world.client.Setup();
				//world.client.printDebug = true;
				world.isServer = false;
				world.SpawnPlayer(world);
				world.SpawnAutonomousProxy();

				waiting = false;
			}
		}
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
