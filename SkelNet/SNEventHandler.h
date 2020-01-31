#pragma once
#include "SDL_stdinc.h"
#include "SNEvent.h"

class SNEventHandler
{
public:
	SNEventHandler() {};

	SNEvent* events[MAX_EVENTS];

	static const int MAX_EVENTS = 20;
	int NUM_EVENTS = 0;
	SNEvent events[MAX_EVENTS];

	void CreateEvent(void(*function)(void), Uint8 eventFlag);
	void RemoveEvent(Uint8 eventFlag);

	void InvokeEvent(Uint8 eventFlag);
};
