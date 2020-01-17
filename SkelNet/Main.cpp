#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>

#include "Engine.h"
#include "Vector.h"
#include "World.h"
#include "Server.h"
#include "Client.h"
#include "Key.h"
#include "UIElement.h"
#include "Canvas.h"

World world;
bool waiting = true;

Canvas canvas;

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
	canvas.drawDebug = true;
	UIElement* rect = canvas.CreateRect({ 30.f, 30.f }, { 40.f,20.f });
	canvas.CreateButton({ 60.f, 80.f }, { 50.f,30.f }, true, Print, &rect->anchor);

		while (!engShouldQuit())
		{
			// Move further down?
			engRender();
			engUpdate();

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
