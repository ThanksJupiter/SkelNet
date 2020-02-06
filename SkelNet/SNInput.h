#pragma once
#include "Vector.h"
class SNInput
{
public:

	Vector2 leftStickDirection;
	Vector2 rightStickDirection;

	bool jump = false;
	bool jumpHeld = false;
	bool attack = false;
	bool downTaunt = false;
	bool upTaunt = false;

	bool restart = false;

	void SetInput();
};

