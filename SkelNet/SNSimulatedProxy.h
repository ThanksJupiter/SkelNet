#pragma once
#pragma once

#include "Vector.h"

class SNWorld;
class SNAnimator;

class SNSimulatedProxy
{
public:
	void Spawn(Vector2 initPos, SNWorld& world);

	void SetPosition(Vector2 newPosition);
	void Draw(float dt);

	bool flip;
	Vector2 previousPosition;
	Vector2 position;
	Vector2 velocity;

	SNWorld* world;

	SNAnimator* animator;
};
