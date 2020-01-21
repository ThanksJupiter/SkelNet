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
	void CheckInput();
	void Draw(float dt);
	void Update(float dt);

	bool flip;
	Vector2 previousPosition;
	Vector2 position;
	Vector2 velocity;

	int health;

	SNWorld* world;
	
	SNAnimator* animator;
	SNUIElement* uiText;
	SNAnchor anchor;
	SNCanvas canvas;
	bool drawDebug;

	SNHitBox hitBox;
};
