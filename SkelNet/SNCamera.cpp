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
	Vector2 camCenter;
	camCenter.x = transform.GetPosition().x + (transform.GetScale().x / 2);
	camCenter.y = transform.GetPosition().y + (transform.GetScale().y / 2);

	/*engSetColor(255, 0, 255);
	engDrawPoint(transform.GetPosition(), 15);
	engDrawArrow(transform.GetPosition(), camCenter);
	engDrawPoint(camCenter, 10);
	engSetColor(0, 0, 0);*/

	Vector2 returnPos = oldPos;
	//returnPos = TranslateVector(oldPos, transform.GetPosition());
	//returnPos = ScaleVector(returnPos, camScale);

	returnPos += camCenter;
	returnPos.x *= camScale;
	returnPos.y *= camScale;

	return returnPos;
}
