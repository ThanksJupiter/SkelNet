#include "SNSprite.h"


SNSprite::SNSprite(int inWidth, int inHeight, SDL_Texture* inTex, int spriteIndex)
{
	width = inWidth;
	height = inHeight;
	texture = inTex;
	
	int pos = height * spriteIndex;
	sheetSourceRect = { 0, pos, width, height };
}

void SNSprite::Notify()
{
	OnAnimNotify();
}
