#pragma once
#include "SDL_stdinc.h"
#include "SNEvent.h"

#define RESPAWN_PLAYER_EVENT 1
#define START_GAME_EVENT 2
#define END_GAME_EVENT 3
#define PLAYER_DIED_EVENT 4
#define REMATCH_EVENT 5

class SNWorld;

class SNEventHandler
{
public:
	SNEventHandler() {};

	static const int MAX_EVENTS = 20;
	int NUM_EVENTS = 0;
	SNEvent events[MAX_EVENTS];
	SNWorld* world;

	void CreateEvent(void (SNWorld::*function)(void), Uint8 eventFlag);
	void RemoveEvent(Uint8 eventFlag);

	void InvokeEvent(Uint8 eventFlag);
};