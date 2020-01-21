#pragma once

typedef struct
{
	Uint8 id;
	Uint16 posX;
	Uint16 posY;
	Uint16 health;
	bool serverAttacked;
	bool serverWasHit;

	bool clientAttacked;
	bool clientWasHit;
} SNDataPacket;