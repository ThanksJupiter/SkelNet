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

void SetupServer()
{
	world.server.Setup();
	world.server.printDebug = false;
	world.isServer = true;
	world.SpawnPlayer(world);
	world.SpawnAutonomousProxy(world);

	waiting = false;
}

void SetupClient()
{
	world.client.Setup();
	world.client.printDebug = false;
	world.isServer = false;
	world.SpawnPlayer(world);
	world.SpawnAutonomousProxy(world);

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
	world.SpawnHitBox({ world.worldSize.x / 2, (world.worldSize.y / 3) }, { 100, 50 });

	SpritesheetData attackSheet = SpritesheetData("SN_Skel_Attack-Sheet.png", 12, 100, 30);
	SpritesheetData idleSheet = SpritesheetData("SN_Skel_Idle-Sheet.png", 4, 32, 32);
	SpritesheetData walkSheet = SpritesheetData("SN_Skel_Walk-Sheet.png", 4, 32, 32);

	SNSprite* idleSprites[4];
	SNSprite* attackSprites[12];
	SNSprite* walkSprites[4];
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
	for (int i = 0; i < walkSheet.numberOfFrames; i++)
	{
		walkSprites[i] = new SNSprite(
			walkSheet.cellWidth,
			walkSheet.cellHeight,
			engLoadTexture(walkSheet.filePath),
			i);
	}

	world.idleAnim = new SNAnimation(idleSprites, 4, engLoadTexture(idleSheet.filePath), .25);
	world.attackAnim = new SNAnimation(attackSprites, 11, engLoadTexture(attackSheet.filePath), .15);
	world.walkAnim = new SNAnimation(walkSprites, 4, engLoadTexture(walkSheet.filePath), .25);

	canvas.Setup(world.worldSize / 2.f, { 100.f, 70.f });
	SNUIElement* rect = canvas.CreateRect({ 30.f, 30.f }, { 40.f,20.f });
	SNUIElement* hostButton = canvas.CreateButton({ 0.f, 40.f }, { 50.f,30.f }, true, SetupServer, &rect->anchor);
	SNUIElement* joinButton = canvas.CreateButton({ 0.f, 100.f }, { 50.f,30.f }, true, SetupClient, &rect->anchor);
	canvas.CreateText({ 0,0 }, "Host", &hostButton->anchor);
	canvas.CreateText({ 0,0 }, "Join", &joinButton->anchor);

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
			world.Update();
			world.Draw(deltaTime);
			//canvas.CheckInteraction();
			//canvas.Draw();
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
