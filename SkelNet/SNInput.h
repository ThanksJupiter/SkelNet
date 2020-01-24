#pragma once
#include "Vector.h"
class SNInput
{
public:

	Vector2 leftStickDirection;
	Vector2 rightStickDirection;

	bool jump = false;
	bool attack = false;

	void SetInput();
};

