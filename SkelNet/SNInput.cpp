#include "SNInput.h"
#include "SNEngine.h"


void SNInput::SetInput()
{
	// not confusing
	leftStickDirection.x = engGetKey(Key::Right) ? 1 : engGetKey(Key::Left)? -1 : engGetJoystickAxis(GamepadAxis::LeftStickX);
	leftStickDirection.y = engGetKey(Key::Up) ? 1 : engGetKey(Key::Down) ? -1 : engGetJoystickAxis(GamepadAxis::LeftStickY);

	jump = engGetKeyDown(Key::V) || engGetKeyDown(Key::Space) || engGetButtonDown(GamepadButton::A) || engGetButtonDown(GamepadButton::RB);
	jumpHeld = engGetKey(Key::Space) || engGetButton(GamepadButton::A) || engGetButton(GamepadButton::RB);

	attack = engGetKeyDown(Key::Z) || engGetKeyDown(Key::X) || engGetButtonDown(GamepadButton::X);
	bBtn = engGetKeyDown(Key::X) || engGetButtonDown(GamepadButton::B);
	yBtn = engGetKeyDown(Key::C) || engGetButtonDown(GamepadButton::Y);

	downTaunt = engGetKeyDown(Key::T) || engGetDPadButtonDown(DPadButton::Down);
	upTaunt = engGetKeyDown(Key::R) || engGetDPadButtonDown(DPadButton::Up);
	rightTaunt = engGetKeyDown(Key::F) || engGetDPadButtonDown(DPadButton::Right);
	leftTaunt = engGetDPadButtonDown(DPadButton::Left);

	restart = engGetButtonDown(GamepadButton::Start) || engGetKeyDown(Key::U);
}
