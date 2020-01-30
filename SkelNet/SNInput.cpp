#include "SNInput.h"
#include "SNEngine.h"


void SNInput::SetInput()
{
	// not confusing
	leftStickDirection.x = engGetKey(Key::Right) ? 1 : engGetKey(Key::Left)? -1 : engGetJoystickAxis(GamepadAxis::LeftStickX);
	leftStickDirection.y = engGetKey(Key::Up) ? 1 : engGetKey(Key::Down) ? -1 : engGetJoystickAxis(GamepadAxis::LeftStickY);

	jump = engGetKeyDown(Key::Space) || engGetButtonDown(GamepadButton::A);
	jumpHeld = engGetKey(Key::Space) || engGetButton(GamepadButton::A);

	attack = engGetKeyDown(Key::X) || engGetButtonDown(GamepadButton::X);

	taunt = engGetKeyDown(Key::T) || engGetDPadButtonDown(DPadButton::Down);
}
