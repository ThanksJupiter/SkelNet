 #pragma once

#include "Vector.h"
#include "SNAnchor.h"
#include "SNCanvas.h"
#include "SNHitBox.h"

class SNWorld;
class SNAnimator;

class SNAutonomousProxy
{
public:
	void Spawn(Vector2 initPos, SNWorld& world);

	void UpdatePosition(float dt);
	void SendData();
	void SetPosition(Vector2 newPosition);
	bool IsGrounded();

	void Attack();
	void CheckAttack();
	void TakeDamage();
	void CheckInput(float dt);
	void Draw(float dt);
	void Update(float dt);
	void FlyBack();

	bool flip;
	Vector2 previousPosition;
	Vector2 position;
	Vector2 velocity;

	bool facingRight;
	Vector2 acceleration;

	int health = 1;
	float accelerationSpeed = 200.0f;
	float minVelocitySpeed = 50.0f;
	float maxVelocitySpeed = 250.0f;
	float minRunSpeed = 160.0f;
	float gravity = 9.82f;
	float gravityMult = 30;
	
	float minFlyBack = 200.f;
	Vector2 flyBackDirection;

	SNWorld* world;

	SNAnimator* animator;
	SNUIElement* uiText;
	SNUIElement* accText;
	SNUIElement* velText;
	SNAnchor anchor;
	SNCanvas canvas;
	bool drawDebug;

	SNHitBox* hitBox;
	SNHitBox* attackBoxR;
	SNHitBox* attackBoxL;
	bool serverAttacked;
	bool serverWasHit;

	bool clientAttacked;
	bool clientWasHit;
};
