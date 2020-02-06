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
	Vector2 returnPos = oldPos;
	returnPos = ScaleVector(returnPos, camScale);
	returnPos = TranslateVector(oldPos, GetCenterPosition());

	return returnPos;
}

Vector2 SNCamera::GetCenterPosition()
{
	Vector2 camCenter;
	camCenter.x = transform.GetPosition().x + (transform.GetScale().x / 2);
	camCenter.y = transform.GetPosition().y + (transform.GetScale().y / 2);
	camCenter = camCenter / engGetRenderScale();
	return camCenter;
}
