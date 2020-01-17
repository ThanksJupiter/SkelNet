 #pragma once

#include "Vector.h"

class SNWorld;
class SNAnimator;

class SNAutonomousProxy
{
public:
	void Spawn(Vector2 initPos, SNWorld& world);

	void UpdatePosition();
	void SetPosition(Vector2 newPosition);
	void CheckInput();
	void Draw();
	void Update();

	bool flip;
	Vector2 previousPosition;
	Vector2 position;
	Vector2 velocity;

	SNWorld* world;
	
	SNAnimator* animator;

};
