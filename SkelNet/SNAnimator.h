#pragma once
#include "SDL_config.h"

struct AnimationInstance;
class Vector2;
class SNSprite;
struct SNAnimation;
struct SDL_Texture;

class SNAnimator
{
public:
	SNAnimator();

	int currentAnimFrame = 0;

	double timer = 0.0f;
	double nextFrameDelay = 0.25;

	float scale = 3.0;

	bool isWalking = false;

	SNAnimation* currentAnimation;
	int direction = 0;

	void DrawAnimation(Vector2 position, bool flipped, float dt);
	void SetCurrentAnimation(SNAnimation* inAnim, int frameCount);
};
