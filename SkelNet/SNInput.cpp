#include "SNInput.h"
#include "SNEngine.h"


void SNInput::SetInput()
{
	// not confusing
	leftStickDirection.x = engGetKey(Key::Right) ? 1 : engGetKey(Key::Left)? -1 : engGetJoystickAxis(GamepadAxis::LeftStickX);

	jump = engGetKeyDown(Key::Space) || engGetButtonDown(GamepadButton::A);
	attack = engGetKeyDown(Key::X) || engGetButtonDown(GamepadButton::X);
}
