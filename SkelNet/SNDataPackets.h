#pragma once
#include "SDL_stdinc.h"

#define TRANSFORM_FLAG 1

#define AP_STATE_FLAG 2
#define SP_STATE_FLAG 4

#define START_FLAG 8
#define RESET_FLAG 16

struct SNTransformPacket
{
	Uint8 flag; 
	int8_t flip;

	Uint16 posX;
	Uint16 posY;
};

struct SNStatePacket
{
	Uint8 flag;
	Uint8 state;
};