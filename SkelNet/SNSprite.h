#pragma once

#include "SDL_rect.h"
#include "Vector.h"

struct SDL_Texture;

class SNSprite
{
public:
	SNSprite() {}
	SNSprite(int inWidth, int inHeight, SDL_Texture* inTex, int spriteIndex);

	int width;
	int height;
	
	SDL_Rect sheetSourceRect;
	SDL_Texture* texture;
};
