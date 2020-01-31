#pragma once
#include "SDL_stdinc.h"

#define TRANSFORM_FLAG 1

#define AP_STATE_FLAG 2
#define SP_STATE_FLAG 4

#define EVENT_FLAG 5

struct SNTransformPacket
{
	Uint8 flag;
	int8_t flip;

	int16_t posX;
	int16_t posY;
};

struct SNStatePacket
{
	Uint8 flag;
	Uint8 state;
};

struct SNEventPacket
{
	Uint8 flag;
	Uint8 eventFlag;
};