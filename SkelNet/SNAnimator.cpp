#include "SNAnimator.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Vector.h"
#include "SNEngine.h"
#include "SNSprite.h"
#include <iostream>
#include "SNAnimation.h"
#include "SNParticleSystem.h"
#include "SNCamera.h"

SNAnimator::SNAnimator()
{

}

void SNAnimator::DrawAnimation(Vector2 position, bool flipped, float dt, float angle)
{
	timer += dt;
	rotation = angle;

	if (timer > currentAnimation->nextFrameDelay)
	{
		timer = 0.0;

		if (currentAnimFrameCount < currentAnimation->frameCount - 1)
		{
			if (currentAnimation->sprites[currentAnimFrameCount]->shouldNotifyWhenPlayed)
			{
				SNAnimation* anim = currentAnimation->sprites[currentAnimFrameCount]->animation;

				world->particleSystem->StartParticleEffect(
					position,
					anim,
					anim->duration,
					flipped,
					scale,
					rotation);
			}

			if (currentAnimation->sprites[currentAnimFrameCount]->shouldPlaySound)
			{
				if (currentAnimation->sprites[currentAnimFrameCount]->audioChannel >= 0)
				{
					world->audioManager->StopChannel(currentAnimation->sprites[currentAnimFrameCount]->audioChannel);
				}
				currentAnimation->sprites[currentAnimFrameCount]->PlaySound(world);
			}

			currentAnimFrameCount++;
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
		world->mainCamera.MakePositionWithCam(position).x - (currentAnimation->sprites[currentAnimFrameCount]->width * scale) / 2,
		world->mainCamera.MakePositionWithCam(position).y - currentAnimation->sprites[currentAnimFrameCount]->height * scale,
		// size
		currentAnimation->sprites[currentAnimFrameCount]->width * scale,
		currentAnimation->sprites[currentAnimFrameCount]->height * scale
	};

	SDL_SetTextureColorMod(currentAnimation->sprites[currentAnimFrameCount]->texture,
		r, g, b);

	engDrawSprite(sourceRect, destinationRect, currentAnimation->sprites[currentAnimFrameCount]->texture, flipped, rotation, { destinationRect.w / 2, destinationRect.h - 40 });
}

void SNAnimator::DrawAnimation(Vector2 position, bool flipped)
{
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
		world->mainCamera.MakePositionWithCam(position).x - (currentAnimation->sprites[currentAnimFrameCount]->width * scale) / 2,
		world->mainCamera.MakePositionWithCam(position).y - currentAnimation->sprites[currentAnimFrameCount]->height * scale,
		// size
		currentAnimation->sprites[currentAnimFrameCount]->width * scale,
		currentAnimation->sprites[currentAnimFrameCount]->height * scale
	};

	SDL_SetTextureColorMod(currentAnimation->sprites[currentAnimFrameCount]->texture,
		r, g, b);

	engDrawSprite(sourceRect, destinationRect, currentAnimation->sprites[currentAnimFrameCount]->texture, flipped);
}

void SNAnimator::IncrementOneFrame()
{
	currentAnimFrameCount++;

	if (currentAnimFrameCount == currentAnimation->frameCount)
	{
		currentAnimFrameCount = currentAnimation->frameCount - 1;
	}
}

void SNAnimator::SetCurrentAnimation(SNAnimation* inAnim, bool oneShot)
{
	if (oneShot)
	{
		previousAnimation = currentAnimation;
		returnToDefaultAnimWhenDone = true;
	}

	currentAnimFrameCount = 0;
	currentAnimation = inAnim;
}

void SNAnimator::SetCurrentAnimation(SNAnimation* inAnim, int startFrame)
{
	currentAnimFrameCount = startFrame;
	currentAnimation = inAnim;
}
