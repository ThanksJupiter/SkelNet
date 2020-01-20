#include "SNSprite.h"

SNSprite::SNSprite(Vector2 size, Vector2 sheetPosition, SDL_Texture* inTex)
{
	
}

SNSprite::SNSprite(int sheetSizeX, int sheetSizeY, int sheetPositionX, int sheetPositionY, SDL_Texture* inTex)
{
	sheetSourceRect =  { sheetSizeX, sheetSizeY, sheetPositionX, sheetPositionY };
	texture = inTex;
}

SNSprite::SNSprite(int inWidth, int inHeight, SDL_Texture* inTex, int spriteIndex)
{
	width = inWidth;
	height = inHeight;
	texture = inTex;
	
	int pos = height * spriteIndex;
	sheetSourceRect = { 0, pos, width, height };
}
