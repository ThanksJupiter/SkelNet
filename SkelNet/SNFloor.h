#pragma once
#include "Vector.h"
#include "SNTransform.h"

class SNWorld;

class SNFloor
{
public:
	void Spawn(Vector2 pos, Vector2 size, SNWorld* world);
	void Draw();

	SNTransform transform;

	SNWorld* world;
};