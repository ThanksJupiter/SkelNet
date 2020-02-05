#pragma once

#include "SDL_rect.h"
#include "Vector.h"
#include <functional>
#include "SNWorld.h"

struct SDL_Texture;
struct SNAnimation;

class SNSprite
{
public:
	SNSprite() {}
	SNSprite(int inWidth, int inHeight, SDL_Texture* inTex, int spriteIndex);

	int width;
	int height;
	
	bool shouldNotifyWhenPlayed = false;
	std::function<void(SNWorld*)> OnAnimNotify;
	SNAnimation* animation;
	void Notify(SNWorld* world);

	SDL_Rect sheetSourceRect;
	SDL_Texture* texture;
};
