#pragma once
#include "SDL_stdinc.h"

#define TRANSFORM_FLAG 1

#define AP_STATE_FLAG 2
#define SP_STATE_FLAG 3

#define EVENT_FLAG 4

#define STRING_FLAG 5

#define HEALTH_FLAG 6

#define DOOT_FLAG 7

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

struct SNStringPacket
{
	Uint8 flag;
	char* string;
};

struct SNHealthPacket
{
	Uint8 flag;
	Uint8 serverHealth;
	Uint8 serverStocks;
	Uint8 clientHealth;
	Uint8 clientStocks;
};

struct SNDootPacket
{
	Uint8 flag;
	Uint8 dootFlag;
};