#include "SNAnimation.h"

SNAnimation::SNAnimation(SNSprite* inSprites[], int inFrameCount, SDL_Texture* inTex, float inDelay)
{
	for (int i = 0; i < inFrameCount; i++)
	{
		sprites[i] = inSprites[i];
	}

	texture = inTex;
	frameCount = inFrameCount;
	nextFrameDelay = inDelay;
}

SNAnimation::~SNAnimation()
{}
