#pragma once
#include <functional>

class SNSprite;
struct SDL_Texture;
class SNWorld;

const static int MAX_FRAME_NUM = 30;

struct SNAnimation
{
	SNAnimation(SNSprite* inSprites[], int inFrameCount, SDL_Texture* inTex, float inDelay, float inDuration);
	~SNAnimation();

	SNSprite* sprites[MAX_FRAME_NUM];
	SDL_Texture* texture;
	float duration;
	int frameCount;
	float nextFrameDelay = .2f;

	void AddDelegateToFrame(int index, std::function<void(SNWorld*)> funct);
};
