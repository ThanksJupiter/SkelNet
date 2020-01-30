#pragma once
#include "SNTransform.h"

class SNCamera
{
public:
	SNCamera();
	~SNCamera();

	SNTransform transform;
	float camScale = 1;
	Vector2 MakePositionWithCam(Vector2 oldPos);
};

