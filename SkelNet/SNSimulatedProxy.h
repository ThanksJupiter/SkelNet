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
	bool isGrounded();

	void SetAnimation(int index);

	bool flip;
	Vector2 previousPosition;
	Vector2 position;
	Vector2 velocity;
	bool facingRight;

	float minFlyBack = 200.f;
	Vector2 flyBackDirection;

	bool isAttacking;

	int health = 1;

	SNWorld* world;

	SNAnimator* animator;
	uint16_t animState = 6;

	SNHitBox* hitBox;
	SNHitBox* attackBoxR;
	SNHitBox* attackBoxL;
	bool didHit;
	
};

void SPDoAttack(SNWorld* world);
