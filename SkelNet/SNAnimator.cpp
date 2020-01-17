#include "SNAnimator.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Vector.h"
#include "SNEngine.h"

SNAnimator::SNAnimator()
{

}

void SNAnimator::SetCurrentAnimationIndex(int index)
{
	
}

void SNAnimator::DrawAnimation(Vector2 position, bool flipped)
{
	Uint32 ticks = SDL_GetTicks();
	Uint32 seconds = ticks / 200;
	Uint32 sprite = seconds % 2; // + offset for first frame & modulus for frames num

	SDL_Rect sourceRect = { sprite * 32, 0, 32, 32 };
	SDL_Rect destinationRect = { position.x - 64, position.y - 128, 128, 128 };

	// don't want to need renderer & texture here mayb

	engDrawSprite(sourceRect, destinationRect, flipped);
}
