#include "SNTransform.h"

SNTransform::SNTransform()
{
	position = { 0,0 };
	facingRight = false;
	scale = { 1,1 };
}

Vector2 SNTransform::SetPosition(Vector2 inPos)
{
	return position = inPos;
}

Vector2 SNTransform::SetPreviousPosition(Vector2 inPrevPos)
{
	return previousPosition = inPrevPos;
}

Vector2 SNTransform::SetScale(Vector2 inScale)
{
	return scale = inScale;
}

Vector2 SNTransform::SetVelocity(Vector2 inVel)
{
	return velocity = inVel;
}

Vector2 SNTransform::SetAcceleration(Vector2 inAcc)
{
	return acceleration = inAcc;
}

bool SNTransform::SetFacingRight(bool inFacingRight)
{
	return facingRight = inFacingRight;
}

Vector2 SNTransform::GetPosition()
{
	return position;
}

Vector2 SNTransform::GetPreviousPosition()
{
	return previousPosition;
}

Vector2 SNTransform::GetScale()
{
	return scale;
}

Vector2 SNTransform::GetVelocity()
{
	return velocity;
}

Vector2 SNTransform::GetAcceleration()
{
	return acceleration;
}

bool SNTransform::GetFacingRight()
{
	return facingRight;
}
