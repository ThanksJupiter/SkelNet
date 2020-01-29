#pragma once
#include "Vector.h"

class SNTransform
{
private:
	Vector2 position;
	Vector2 previousPosition;
	Vector2 scale;
	Vector2 velocity;
	Vector2 acceleration;
	bool facingRight;

public:
	SNTransform();
	Vector2 GetPosition();
	Vector2 GetPreviousPosition();
	Vector2 GetScale();
	Vector2 GetVelocity();
	Vector2 GetAcceleration();
	bool GetFacingRight();

	Vector2 SetPosition(Vector2 inPos);
	Vector2 SetPreviousPosition(Vector2 inPos);
	Vector2 SetScale(Vector2 inPos);
	Vector2 SetVelocity(Vector2 inPos);
	Vector2 SetAcceleration(Vector2 inPos);
	bool SetFacingRight(bool inFacingRight);
};
