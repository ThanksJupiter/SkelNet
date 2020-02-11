#include "SNInput.h"
#include "SNEngine.h"


void SNInput::SetInput()
{
	// not confusing
	leftStickDirection.x = engGetKey(Key::Right) ? 1 : engGetKey(Key::Left)? -1 : engGetJoystickAxis(GamepadAxis::LeftStickX, playerIndex);
	leftStickDirection.y = engGetKey(Key::Up) ? 1 : engGetKey(Key::Down) ? -1 : engGetJoystickAxis(GamepadAxis::LeftStickY, playerIndex);

	jump = engGetKeyDown(Key::V) || engGetKeyDown(Key::Space) || engGetButtonDown(GamepadButton::A, playerIndex) || engGetButtonDown(GamepadButton::RB, playerIndex);
	jumpHeld = engGetKey(Key::Space) || engGetButton(GamepadButton::A, playerIndex) || engGetButton(GamepadButton::RB, playerIndex);

	attack = engGetKeyDown(Key::Z) || engGetKeyDown(Key::X) || engGetButtonDown(GamepadButton::X, playerIndex);
	bBtn = engGetKeyDown(Key::X) || engGetButtonDown(GamepadButton::B, playerIndex);
	yBtn = engGetKeyDown(Key::C) || engGetButtonDown(GamepadButton::Y, playerIndex);

	downTaunt = engGetKeyDown(Key::T) || engGetDPadButtonDown(DPadButton::Down, playerIndex);
	upTaunt = engGetKeyDown(Key::R) || engGetDPadButtonDown(DPadButton::Up, playerIndex);
	rightTaunt = engGetKeyDown(Key::F) || engGetDPadButtonDown(DPadButton::Right, playerIndex);
	leftTaunt = engGetDPadButtonDown(DPadButton::Left, playerIndex);

	restart = engGetButtonDown(GamepadButton::Start, playerIndex) || engGetKeyDown(Key::U);
}
