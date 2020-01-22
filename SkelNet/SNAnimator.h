#pragma once
#include "SDL_config.h"

struct AnimationInstance;
class Vector2;
class SNSprite;
struct SNAnimation;
struct SDL_Texture;

typedef enum
{
	ANIMS_IDLE = 0x00000001,
	ANIMS_WALK = 0x00000002,
	ANIMS_ATTACK = 0x00000004,
	ANIMS_KNOCKBACK = 0x00000008
} ANIM_State;

class SNAnimator
{
public:
	SNAnimator();

	int currentAnimFrameCount = 0;

	double timer = 0.0f;
	double nextFrameDelay = 0.25;

	ANIM_State currentState;
	bool movementLocked = false;
	bool returnToPreviousAnimWhenDone = false;

	float scale = 3.0;

	bool isWalking = false;

	SNAnimation* currentAnimation;
	SNAnimation* previousAnimation;
	int direction = 0;

	void DrawAnimation(Vector2 position, bool flipped, float dt);
	void SetCurrentAnimation(SNAnimation* inAnim, bool oneShot = false);
	bool IsCurrentAnimationDonePlaying();
};
