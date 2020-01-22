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
	void TakeDamage();
	void CheckInput(float dt);
	void Draw(float dt);
	void Update(float dt);

	bool flip;
	Vector2 previousPosition;
	Vector2 position;
	Vector2 velocity;

	bool facingRight;
	Vector2 acceleration;

	int health;
	float accelerationSpeed = 150.0f;
	float minVelocitySpeed = 100.0f;
	float maxVelocitySpeed = 250.0f;
	float minRunSpeed = 160.0f;
	float gravity = 9.82f;
	float gravityMult = 30;

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
