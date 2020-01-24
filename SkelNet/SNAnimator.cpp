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

void SNAnimator::DrawAnimation(Vector2 position, bool flipped, float dt, float angle)
{
	timer += dt;

	if (timer > currentAnimation->nextFrameDelay)
	{
		timer = 0.0;
		
		if (currentAnimFrameCount < currentAnimation->frameCount - 1)
		{
			currentAnimFrameCount++;

			if (currentAnimation->sprites[currentAnimFrameCount]->shouldNotifyWhenPlayed)
			{
				currentAnimation->sprites[currentAnimFrameCount]->Notify(world);
			}
		}
		else
		{
			currentAnimFrameCount = 0;
			movementLocked = false;
			if (returnToDefaultAnimWhenDone)
			{
				SetCurrentAnimation(defaultAnimation);
				returnToDefaultAnimWhenDone = false;
			}
		}
	}

	int width = currentAnimation->sprites[currentAnimFrameCount]->width;
	SDL_Rect sourceRect = 
	{ 
		0, 
		currentAnimation->sprites[currentAnimFrameCount]->height * currentAnimFrameCount,
		currentAnimation->sprites[currentAnimFrameCount]->width, 
		currentAnimation->sprites[currentAnimFrameCount]->height 
	};

	SDL_Rect destinationRect = 
	{ 
		// position
		position.x - (currentAnimation->sprites[currentAnimFrameCount]->width * scale) / 2,
		position.y - currentAnimation->sprites[currentAnimFrameCount]->height * scale,
		// size
		currentAnimation->sprites[currentAnimFrameCount]->width * scale,
		currentAnimation->sprites[currentAnimFrameCount]->height * scale
	};

	// debug square
	/*SNSprite* curSprite = currentAnimation->sprites[currentAnimFrameCount];
	engDrawRect
	(
		curSprite->width * scale,
		curSprite->height * scale,
		position.x - (curSprite->width * scale) / 2,
		position.y - curSprite->height * scale
	);*/

	engDrawPoint({ (float)destinationRect.x + (destinationRect.w / 2), (float)destinationRect.y + destinationRect.h - 40}, 5);
	engDrawSprite(sourceRect, destinationRect, currentAnimation->sprites[currentAnimFrameCount]->texture, flipped, rotation, {destinationRect.w / 2, destinationRect.h - 40});
}

void SNAnimator::SetCurrentAnimation(SNAnimation* inAnim, bool oneShot /*= false*/)
{
	if (oneShot)
	{
		previousAnimation = currentAnimation;
		returnToDefaultAnimWhenDone = true;
	}

	currentAnimFrameCount = 0;
	currentAnimation = inAnim;
}
