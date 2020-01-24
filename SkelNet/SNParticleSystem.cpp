#include "SNParticleSystem.h"

SNParticleSystem::SNParticleSystem()
{

}

SNParticleEffect* SNParticleSystem::ReuseParticleEffect()
{
	/*for (int i = 0; i < particleEffects.size(); i++)
	{
		if (!&particleEffects[i])
			continue;

		if (!&particleEffects[i]->isUsed)
		{
			return particleEffects[i];
		}
		else
		{
			return new SNParticleEffect();
		}
	}*/
	return new SNParticleEffect();
}

//call when you want to start the playback of one animation for a set duration at a location
SNParticleEffect* SNParticleSystem::StartParticleEffect(Vector2 position, SNAnimation* animation, float duration, bool flipped)
{
	SNParticleEffect* newEffect = ReuseParticleEffect();
	if (newEffect)
	{
		newEffect->currentPlayDuration = 0;
		newEffect->playDuration = duration;
		newEffect->position = position;
		newEffect->particleAnimation = animation;
		newEffect->flipped = flipped;

		newEffect->particleAnimator = SNAnimator();

		particleEffects.push_back(newEffect);
		newEffect->particleAnimator.SetCurrentAnimation(newEffect->particleAnimation);
		
		return newEffect;
	}
}

void SNParticleSystem::UpdateParticles(float dt)
{
	for (int i = 0; i < particleEffects.size(); i++)
	{
		if (!&particleEffects[i] || !&particleEffects[i]->particleAnimator)
			continue;

		particleEffects[i]->currentPlayDuration += dt;
		if (particleEffects[i]->currentPlayDuration < particleEffects[i]->playDuration)
		{
			particleEffects[i]->particleAnimator.DrawAnimation(particleEffects[i]->position, particleEffects[i]->flipped, dt);
		}
		else
		{
			particleEffects[i]->isUsed = false;
		}
	}
}
