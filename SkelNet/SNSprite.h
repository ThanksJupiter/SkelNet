#pragma once

#include "SDL_rect.h"
#include "Vector.h"

struct SDL_Texture;

class SNSprite
{
public:
	SNSprite() {}
	SNSprite(int inWidth, int inHeight, SDL_Texture* inTex, int spriteIndex);
	SNSprite(int inWidth, int inHeight, SDL_Rect inSrcRect, SDL_Texture* inTex) : width(inWidth), height(inHeight), sheetSourceRect(inSrcRect), texture(inTex) {}
	SNSprite(Vector2 size, Vector2 sheetPosition, SDL_Texture* inTex);
	SNSprite(int sheetSizeX, int sheetSizeY, int sheetPositionX, int sheetPositionY, SDL_Texture* inTex);

	int width;
	int height;
	
	SDL_Rect sheetSourceRect;
	SDL_Texture* texture;
};
