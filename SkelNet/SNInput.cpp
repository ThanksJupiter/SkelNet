#include "SNInput.h"
#include "SNEngine.h"


void SNInput::SetInput()
{
	// not confusing double ternary I promise
	leftStickDirection.x = engGetKey(Key::Right) ? 1 : engGetKey(Key::Left)? -1 : 0;
	leftStickDirection.x = engGetJoystickAxis(GamepadAxis::LeftStickX);

	jump = engGetKeyDown(Key::Space) || engGetButtonDown(GamepadButton::A);
	attack = engGetKeyDown(Key::X) || engGetButtonDown(GamepadButton::X);
}
