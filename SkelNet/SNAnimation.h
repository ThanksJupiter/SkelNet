#pragma once

#include "SNSprite.h"

class SNAnimation
{
public:
	SNAnimation();
	
	static const int MAX_ANIMATION_FRAMES = 10;
	int numberOfFrames;

	SNSprite animationFrames[MAX_ANIMATION_FRAMES];
};
