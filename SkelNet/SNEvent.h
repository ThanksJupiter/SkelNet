#pragma once
#include "SDL_stdinc.h"

class SNWorld;

class SNEvent
{
public:
	SNEvent() {};

	void Setup(SNWorld* world, void (SNWorld::*function)(void));
	void Invoke();

	bool isUsed;
	Uint8 flag;
	SNWorld* world;

private:
	void (SNWorld::*delegatedFunction)(void);
};
