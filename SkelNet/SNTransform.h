#pragma once
#include "Vector.h"
#include "SDL_config.h"

class SNCamera;

class SNTransform
{
private:
	Vector2 position;
	Vector2 previousPosition;
	Vector2 scale;
	Vector2 velocity;
	Vector2 acceleration;
	bool facingRight = true;

public:
	SNTransform();
	Vector2 GetPosition();
	Vector2 GetPreviousPosition();
	Vector2 GetScale();
	Vector2 GetVelocity();
	Vector2 GetAcceleration();
	bool GetFacingRight();

	void SetPosition(Vector2 inPos);
	void SetPreviousPosition(Vector2 inPos);
	void SetScale(Vector2 inPos);
	void SetVelocity(Vector2 inPos);
	void SetAcceleration(Vector2 inPos);
	void SetFacingRight(bool inFacingRight);
	void SetFacingRight(int8_t in);
};
