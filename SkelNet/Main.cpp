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

	//canvas.Setup(world.worldSize / 2.f, { 100.f, 70.f });
	//SNUIElement* rect = canvas.CreateRect({ 30.f, 30.f }, { 40.f,20.f });
	//canvas.CreateButton({ 60.f, 80.f }, { 50.f,30.f }, true, Print, &rect->anchor);
	//canvas.CreateText({ 60.f, 80.f }, "Heasasass", &rect->anchor, { 20.f, 60.f });

	SpritesheetData attackSheet = SpritesheetData("SN_Skel_Attack-Sheet.png", 11, 100, 30);
	SpritesheetData idleSheet = SpritesheetData("SN_Skel_Idle-Sheet.png", 4, 32, 32);

	SNSprite* idleSprites[4];
	SNSprite* attackSprites[11];
	for (int i = 0; i < idleSheet.numberOfFrames; i++)
	{
		idleSprites[i] = new SNSprite(
			idleSheet.cellWidth,
			idleSheet.cellHeight,
			engLoadTexture(idleSheet.filePath),
			i);
	}

	for (int i = 0; i < attackSheet.numberOfFrames; i++)
	{
		attackSprites[i] = new SNSprite(
			attackSheet.cellWidth, 
			attackSheet.cellHeight,
			engLoadTexture(attackSheet.filePath),
			i);
	}

	world.idleAnim = new SNAnimation(idleSprites, 4, engLoadTexture(idleSheet.filePath));
	world.attackAnim = new SNAnimation(attackSprites, 11, engLoadTexture(attackSheet.filePath));

	// delta time
	uint64_t NOW = SDL_GetPerformanceCounter();
	uint64_t LAST = 0;
	double deltaTime = 0;
	float timer = 0;

	while (!engShouldQuit())
	{
		// Move further down?
		engRender();
		engUpdate();

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000) / (double)SDL_GetPerformanceFrequency();

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
			world.Draw(deltaTime * 0.001);
			//canvas.CheckInteraction();
			//canvas.Draw();

			if (engGetKeyDown(Key::A) && world.isServer == true)
			{
				world.server.AcceptConnection();
			}

			if (engGetKeyDown(Key::D))
			{
				//canvas.drawDebug = !canvas.drawDebug;
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
				world.SpawnAutonomousProxy(world);

				waiting = false;
			}

			if (engGetKeyDown(Key::C))
			{
				world.client.Setup();
				//world.client.printDebug = true;
				world.isServer = false;
				world.SpawnPlayer(world);
				world.SpawnAutonomousProxy(world);

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
