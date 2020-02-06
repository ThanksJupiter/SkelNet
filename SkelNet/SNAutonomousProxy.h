 #pragma once

#include "Vector.h"
#include "SNAnchor.h"
#include "SNCanvas.h"
#include "SNHitBox.h"
#include "SNTransform.h"
#include "SDL_stdinc.h"

class SNWorld;
class SNAnimator;
class SNFiniteStateMachine;
class SNInput;
struct SNFSMData;

class SNAutonomousProxy
{
public:
	void Spawn(Vector2 initPos, SNWorld& world);

	void UpdatePosition(float dt);
	void SendTransformData();
	void SetPosition(Vector2 newPosition);
	bool IsGrounded();

	void SendEnterAttackState();
	void CheckAttack();
	void TakeDamage();
	void SetState(Uint8 index);
	void CheckInput(float dt);
	void Draw(float dt, SNCamera* cam);
	void Update(float dt);
	void ForcesTimeIntegration(float dt);

	void SetDirection();

	void InitializeFSM();
	void SendSPState(Uint8 state);
	void SendAPState(Uint8 state);
	void EnterState(Uint8 state);

	void DoAttack();
	SNUIElement* stateText;
	SNUIElement* spStateText;
	SNUIElement* velocityText;

	SNFiniteStateMachine* stateMachine;
	SNFSMData* fsmData;
	SNInput* playerInput;
	void FlyBack();

	SNTransform transform;
	float drag = 1.5f;

	bool sendTransformToggle;
	bool flip;

	bool inputEnabled = true;

	int health = 1;
	bool isInvulnerable = false;
	float timer = 0.0f;
	float regainVulnerabilityDelay = 2.0f;
	Uint8 currentStocks = 0;
	Uint8 maxStocks = 3;

	float accelerationSpeed = 270.0f;
	float minVelocitySpeed = 50.0f;
	float maxVelocitySpeed = 250.0f;
	float minRunSpeed = 160.0f;
	float gravity = 9.82f;
	float gravityMult = 20;
	float fallGravityMult = 60;
	float fastFallGravityMult = 35;
	float airControlMult = .6f;
	
	float minFlyBack = 200.f;
	Vector2 flyBackDirection;

	SNWorld* world;

	SNAnimator* animator;
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
	void Reset();
};


