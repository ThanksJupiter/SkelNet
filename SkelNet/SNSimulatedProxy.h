#pragma once
#pragma once

#include "Vector.h"
#include "SNHitBox.h"
#include "SNTransform.h"
#include "SDL_stdinc.h"

class SNWorld;
class SNAnimator;
class SNFiniteStateMachine;
struct SNFSMData;

class SNSimulatedProxy
{
public:
	void Spawn(Vector2 initPos, SNWorld& world);
	void Update(float dt);

	void SetPosition(Vector2 newPosition);
	void Draw(float dt, SNCamera* cam);

	void ServerCheckAttack();
	void PlayAttackAnim();
	void TakeDamage();
	void FlyBack();
	void SetHealthAndStocks(Uint8 newHealth, Uint8 newStocks);
	bool isGrounded();

	void SetAnimation(int index);
	void SetState(Uint8 index);
	void PlayDoot(Uint8 dootFlag);

	void InitializeFSM();

	void DoAttack();
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
	bool isInvulnerable = false;
	float timer = 0.0f;
	float regainVulnerabilityDelay = 2.0f;

	Uint8 health = 1;
	Uint8 currentStocks = 0;
	Uint8 maxStocks = 3;

	SNWorld* world;

	SNAnimator* animator;
	uint16_t animState = 6;

	SNHitBox* hitBox;
	SNHitBox* attackBoxR;
	SNHitBox* attackBoxL;
	bool didHit;
	
	void Reset();
};


