#pragma once
#include "SNAnimation.h"
#include "SNAnimator.h"
#include "Vector.h"
#include <vector>

struct SNParticleEffect
{
	SNAnimation* particleAnimation;
	SNAnimator particleAnimator;

	Vector2 position;
	float playDuration;
	float currentPlayDuration;

	bool flipped;

	bool isUsed;
};

class SNParticleSystem
{
public:
	SNParticleSystem();

	SNParticleEffect* ReuseParticleEffect();
	
	SNParticleEffect* StartParticleEffect(Vector2 position, SNAnimation* animation, float duration, bool flipped = false);
	void UpdateParticles(float dt);
	std::vector<SNParticleEffect*> particleEffects;

};
