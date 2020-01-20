 #pragma once

#include "Vector.h"
#include "SNAnchor.h"
#include "SNCanvas.h"

class SNWorld;
class SNAnimator;

class SNAutonomousProxy
{
public:
	void Spawn(Vector2 initPos, SNWorld& world);

	void UpdatePosition();
	void SendData();
	void SetPosition(Vector2 newPosition);
	bool IsGrounded();
	void CheckInput();
	void Draw(float dt);
	void Update();

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
};
