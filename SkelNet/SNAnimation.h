#pragma once
#include <functional>

class SNSprite;
struct SDL_Texture;

const static int MAX_FRAME_NUM = 30;

struct SNAnimation
{
	SNAnimation(SNSprite* inSprites[], int inFrameCount, SDL_Texture* inTex, float inDelay);
	~SNAnimation();

	SNSprite* sprites[MAX_FRAME_NUM];
	SDL_Texture* texture;
	int frameCount;
	float nextFrameDelay = .2f;

	void AddDelegateToFrame(int index, std::function<void()> funct);
};
