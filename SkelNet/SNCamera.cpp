#include "SNCamera.h"
#include "SNMath.h"
#include "SNEngine.h"

SNCamera::SNCamera()
{
}


SNCamera::~SNCamera()
{
}
	
Vector2 SNCamera::MakePositionWithCam(Vector2 oldPos)
{
	

	/*engSetColor(255, 0, 255);
	engDrawPoint(transform.GetPosition(), 15);
	engDrawArrow(transform.GetPosition(), camCenter);
	engDrawPoint(camCenter, 10);
	engSetColor(0, 0, 0);*/

	Vector2 returnPos = oldPos;
	returnPos = ScaleVector(returnPos, camScale);
	returnPos = TranslateVector(oldPos, GetCenterPosition());

	//returnPos += camCenter;
	//returnPos.x *= camScale;
	//returnPos.y *= camScale;

	return returnPos;
}

Vector2 SNCamera::GetCenterPosition()
{
	Vector2 camCenter;
	camCenter.x = transform.GetPosition().x + (transform.GetScale().x / 2);
	camCenter.y = transform.GetPosition().y + (transform.GetScale().y / 2);
	return camCenter;
}
