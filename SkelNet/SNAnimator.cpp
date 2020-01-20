#include "SNAnimator.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Vector.h"
#include "SNEngine.h"
#include "SNSprite.h"
#include <iostream>
#include "SNAnimation.h"

SNAnimator::SNAnimator()
{
	
}

void SNAnimator::DrawAnimation(Vector2 position, bool flipped, float dt)
{
	timer += dt;

	if (timer > nextFrameDelay)
	{
		timer = 0.0;
		
		if (currentAnimFrame < currentAnimation->frameCount - 1)
		{
			currentAnimFrame++;
		}
		else
		{
			currentAnimFrame = 0;
		}
	}

	int width = currentAnimation->sprites[currentAnimFrame]->width;
	SDL_Rect sourceRect = 
	{ 
		0, 
		currentAnimation->sprites[currentAnimFrame]->height * currentAnimFrame,
		currentAnimation->sprites[currentAnimFrame]->width, 
		currentAnimation->sprites[currentAnimFrame]->height 
	};

	SDL_Rect destinationRect = 
	{ 
		// position
		position.x - (currentAnimation->sprites[currentAnimFrame]->width * scale) / 2,
		position.y - currentAnimation->sprites[currentAnimFrame]->height * scale,
		// size
		currentAnimation->sprites[currentAnimFrame]->width * scale,
		currentAnimation->sprites[currentAnimFrame]->height * scale
	};

	/*engDrawRect
	(
		attackAnimSprites[currentAnimFrame]->width * scale,
		attackAnimSprites[currentAnimFrame]->height * scale,
		position.x - (attackAnimSprites[currentAnimFrame]->width * scale) / 2,
		position.y - attackAnimSprites[currentAnimFrame]->height * scale
	);*/

	engDrawSprite(sourceRect, destinationRect, currentAnimation->sprites[currentAnimFrame]->texture, flipped);
}

void SNAnimator::SetCurrentAnimation(SNAnimation* inAnim, int frameCount)
{
	currentAnimFrame = 0;
	currentAnimation = inAnim;
}
