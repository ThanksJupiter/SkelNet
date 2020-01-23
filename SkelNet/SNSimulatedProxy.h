#pragma once
#pragma once

#include "Vector.h"
#include "SNHitBox.h"

class SNWorld;
class SNAnimator;

class SNSimulatedProxy
{
public:
	void Spawn(Vector2 initPos, SNWorld& world);

	void SetPosition(Vector2 newPosition);
	void Draw(float dt);

	void ServerCheckAttack();
	void PlayAttackAnim();
	void TakeDamage();
	void FlyBack();

	bool flip;
	Vector2 previousPosition;
	Vector2 position;
	Vector2 velocity;
	bool facingRight;

	float minFlyBack = 500.f;
	Vector2 flyBackDirection;

	bool isAttacking;

	int health;

	SNWorld* world;

	SNAnimator* animator;

	SNHitBox* hitBox;
	SNHitBox* attackBoxR;
	SNHitBox* attackBoxL;
	bool didHit;
	
};
