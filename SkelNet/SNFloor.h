#pragma once
#include "Vector.h"

class SNFloor
{
public:
	void Spawn(Vector2 pos, Vector2 size);
	void Draw();

	Vector2 position;
	Vector2 size;
};