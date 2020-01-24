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

	bool isUsed;
};

class SNParticleSystem
{
public:
	SNParticleSystem();

	SNParticleEffect* ReuseParticleEffect();

	SNParticleEffect* StartParticleEffect(Vector2 position, SNAnimation* animation, float duration);
	
	void PlayParticleEffect(float dt);
	std::vector<SNParticleEffect*> particleEffects;

};
