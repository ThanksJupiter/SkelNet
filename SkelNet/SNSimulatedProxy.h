#pragma once
#pragma once

#include "Vector.h"

class SNWorld;
class SNAnimator;

class SNSimulatedProxy
{
public:
	void Spawn(Vector2 initPos);

	void SetPosition(Vector2 newPosition);
	void Draw();

	bool flip;
	Vector2 previousPosition;
	Vector2 position;
	Vector2 velocity;

	int health;

	SNWorld* world;

	SNAnimator* animator;
};
