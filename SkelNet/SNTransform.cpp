#include "SNTransform.h"
#include "SNMath.h"
#include "SNCamera.h"

SNTransform::SNTransform()
{

}

//Set Functions

void SNTransform::SetPosition(Vector2 inPos)
{
	position = inPos;
	//position = TranslateVector(inPos);
}

void SNTransform::SetPreviousPosition(Vector2 inPrevPos)
{
	previousPosition = inPrevPos;
}

void SNTransform::SetScale(Vector2 inScale)
{
	scale = inScale;
}

void SNTransform::SetVelocity(Vector2 inVel)
{
	velocity = inVel;
}

void SNTransform::SetAcceleration(Vector2 inAcc)
{
	acceleration = inAcc;
}

void SNTransform::SetFacingRight(bool inFacingRight)
{
	facingRight = inFacingRight;
}

void SNTransform::SetFacingRight(int8_t in)
{
	facingRight = in > 0 ? true : false;
}

//Get Functions

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
