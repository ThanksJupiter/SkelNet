#pragma once
#include "SDL_stdinc.h"

class SNEvent
{
public:
	SNEvent() {};
	SNEvent(void(*function)(void));

	void Setup(void(*function)(void));
	void Invoke();

	bool isUsed;
	Uint8 flag;

private:
	void(*delegatedFunction)(void);
};