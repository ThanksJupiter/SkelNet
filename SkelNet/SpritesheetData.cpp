#include "SpritesheetData.h"
#include "SNAnimation.h"
#include "SNSprite.h"

SpritesheetData::SpritesheetData(const char* inPath, int inFrameNo, int inWidth, int inHeight)
{
	filePath = inPath;
	numberOfFrames = inFrameNo;
	cellWidth = inWidth;
	cellHeight = inHeight;
}

SNAnimation* SpritesheetData::CreateAnimation(SNSprite* inSprites[], int frameCount, SDL_Texture* inTex, float frameDelay)
{
	for (int i = 0; i < frameCount; i++)
	{
		inSprites[i] = new SNSprite(
			cellWidth,
			cellHeight,
			inTex,
			i);
	}

	return new SNAnimation(inSprites, frameCount, inTex, frameDelay);
}
