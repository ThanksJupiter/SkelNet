#include "SNParticleSystem.h"
#include "SNCamera.h"

SNParticleSystem::SNParticleSystem()
{

}

SNParticleEffect* SNParticleSystem::ReuseParticleEffect()
{
	for (int i = 0; i < particleEffects.size(); i++)
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
	}

	return new SNParticleEffect();
}

//call when you want to start the playback of one animation for a set duration at a location
SNParticleEffect* SNParticleSystem::StartParticleEffect(Vector2 pos, SNAnimation* animation, float duration, bool flipped, float scale, float rot)
{
	SNParticleEffect* newEffect = ReuseParticleEffect();
	if (newEffect)
	{
		newEffect->currentPlayDuration = 0;
		newEffect->playDuration = duration;
		newEffect->position = pos;
		newEffect->rotation = rot;
		newEffect->particleAnimation = animation;
		newEffect->flipped = flipped;

		newEffect->particleAnimator = SNAnimator();
		newEffect->particleAnimator.scale = scale;
		particleEffects.push_back(newEffect);
		newEffect->particleAnimator.SetCurrentAnimation(newEffect->particleAnimation);

		return newEffect;
	}
}

void SNParticleSystem::UpdateParticles(float dt, SNCamera* cam)
{
	for (int i = 0; i < particleEffects.size(); i++)
	{
		if (!&particleEffects[i] || !&particleEffects[i]->particleAnimator)
			continue;

		particleEffects[i]->currentPlayDuration += dt;
		if (particleEffects[i]->currentPlayDuration < particleEffects[i]->playDuration)
		{
			particleEffects[i]->particleAnimator.DrawAnimation(particleEffects[i]->position - cam->transform.GetPosition(), particleEffects[i]->flipped, dt, particleEffects[i]->rotation);
		}
		else
		{
			particleEffects[i]->isUsed = false;
		}
	}
}
