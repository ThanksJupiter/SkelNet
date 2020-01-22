#pragma once

typedef struct
{
	Uint8 id;
	Uint16 posX;
	Uint16 posY;
	Uint16 health;

	Uint8 serverAttacked;
	Uint8 serverWasHit;

	Uint8 clientAttacked;
	Uint8 clientWasHit;
} SNDataPacket;