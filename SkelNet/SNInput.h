#pragma once
#include "Vector.h"
#include "Key.h"

#define INPUT_JUMP 1 << 0
#define INPUT_JUMP_HELD 1 << 1
#define INPUT_ATTACK 1 << 3

class SNInput
{
public:

	int playerIndex;

	Vector2 leftStickDirection;
	Vector2 rightStickDirection;

	bool jump = false;
	bool jumpHeld = false;
	bool attack = false;

	bool bBtn = false;
	bool yBtn = false;

	bool downTaunt = false;
	bool upTaunt = false;
	bool rightTaunt = false;
	bool leftTaunt = false;

	bool restart = false;

	void SetInput();
};

struct InputState
{
	bool pressed;
	int frameNum;
};

struct AxisState
{
	float value;
};

struct SNInputStatesSet
{
	static InputState buttonStates[unsigned int(GamepadButton::MAX)];

	static AxisState axisStates[(unsigned int)GamepadAxis::MAX];
	static InputState dpadStates[(unsigned int)DPadButton::MAX];
};
