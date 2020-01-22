#pragma once

#include "SDL_rect.h"
#include "Vector.h"
#include <functional>

struct SDL_Texture;

class SNSprite
{
public:
	SNSprite() {}
	SNSprite(int inWidth, int inHeight, SDL_Texture* inTex, int spriteIndex);

	int width;
	int height;
	
	bool shouldNotifyWhenPlayed = false;
	std::function<void()> OnAnimNotify;
	void Notify();

	SDL_Rect sheetSourceRect;
	SDL_Texture* texture;
};
