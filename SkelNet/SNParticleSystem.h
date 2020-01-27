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
	float rotation;

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
	
	SNParticleEffect* StartParticleEffect(Vector2 pos, SNAnimation* animation, float duration, bool flipped, float scale = 3, float rot = 0.f);
	void UpdateParticles(float dt);
	std::vector<SNParticleEffect*> particleEffects;

};
