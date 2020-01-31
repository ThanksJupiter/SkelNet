#pragma once
#include "SDL_stdinc.h"

#define TRANSFORM_FLAG 1

#define AP_STATE_FLAG 2
#define SP_STATE_FLAG 4

#define START_FLAG 8
#define RESET_FLAG 16

struct SNTransformPacket
{
	int8_t flag; 
	int8_t flip;

	int16_t posX;
	int16_t posY;
};

struct SNStatePacket
{
	int8_t flag;
	int8_t state;
};