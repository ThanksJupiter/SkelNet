#pragma once
#pragma once

#include "Vector.h"
#include "SNHitBox.h"
#include "SNTransform.h"

class SNWorld;
class SNAnimator;
class SNFiniteStateMachine;
struct SNFSMData;

class SNSimulatedProxy
{
public:
	void Spawn(Vector2 initPos, SNWorld& world);

	void SetPosition(Vector2 newPosition);
	void Draw(float dt, SNCamera* cam);

	void ServerCheckAttack();
	void PlayAttackAnim();
	void TakeDamage();
	void FlyBack();
	bool isGrounded();

	void SetAnimation(int index);
	void SetState(int index);

	void InitializeFSM();

	SNFiniteStateMachine* stateMachine;
	SNFSMData* fsmData;

	SNTransform transform;

	bool flip;
	//Vector2 previousPosition;
	//Vector2 position;
	Vector2 previousVelocity;
	//Vector2 velocity;
	//bool facingRight;

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
	
	void Reset();
};

void SPDoAttack(SNWorld* world);
