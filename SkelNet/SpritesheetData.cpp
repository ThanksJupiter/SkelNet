#include "SpritesheetData.h"
#include "SNAnimation.h"
#include "SNSprite.h"
#include "SNEngine.h"

SpritesheetData::SpritesheetData(const char* inPath, int inFrameNo, int inWidth, int inHeight)
{
	filePath = inPath;
	numberOfFrames = inFrameNo;
	cellWidth = inWidth;
	cellHeight = inHeight;
}

SNAnimation* SpritesheetData::CreateAnimation(SNSprite* inSprites[], float frameDelay)
{
	SDL_Texture* texture = engLoadTexture(filePath);
	
	duration = numberOfFrames * frameDelay;

	for (int i = 0; i < numberOfFrames; i++)
	{
		inSprites[i] = new SNSprite(
			cellWidth,
			cellHeight,
			texture,
			i);
	}

	return new SNAnimation(inSprites, numberOfFrames, texture, frameDelay, duration);
}
