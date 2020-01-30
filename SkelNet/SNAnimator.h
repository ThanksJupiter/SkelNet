#pragma once
#include "SDL_config.h"

#define IDLE_ANIM 0
#define WALK_ANIM 1
#define RUN_ANIM 2
#define ATTACK_ANIM 3
#define JUMP_ANIM 4
#define KNOCKBACK_ANIM 5
#define TAUNT_ANIM 6
#define MAX_ANIM 7

struct AnimationInstance;
class Vector2;
class SNSprite;
struct SNAnimation;
struct SDL_Texture;
class SNWorld;

class SNAnimator
{
public:
	SNAnimator();

	int currentAnimFrameCount = 0;

	double timer = 0.0f;
	double nextFrameDelay = 0.25;

	bool movementLocked = false;
	bool returnToDefaultAnimWhenDone = false;

	float scale = 3.0;
	float rotation = 0.f;

	bool isWalking = false;
	bool isRunning = false;

	bool doManualAnimationCycling = false;

	SNAnimation* currentAnimation;
	SNAnimation* previousAnimation;
	SNAnimation* defaultAnimation;
	int direction = 0;

	SNWorld* world;

	void DrawAnimation(Vector2 position, bool flipped);
	void IncrementOneFrame();
	void DrawAnimation(Vector2 position, bool flipped, float dt, float angle = 0);
	void SetCurrentAnimation(SNAnimation* inAnim, bool oneShot = false);
};
