#pragma once

/* All codes are scancodes */
enum class Key : unsigned short
{
	Undefined = 0x0,
	A = 0x4,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,
	NUM0,

	Return,
	Escape,
	BackSpace,
	Tab,
	Space,

	Minus,
	Equals,
	LeftBracket,
	RightBracket,
	BackSlash,

	NONUSHASH,

	SemiColon,
	Apostrophe,
	Grave,

	Comma,
	Period,
	Slash,
	
	Capslock,

	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,

	PrintScreen,
	ScrollLock,
	Pause,
	Insert,

	Home,
	PageUp,
	Delete,
	End,
	PageDown,
	Right,
	Left,
	Down,
	Up,

	NumlockClear,

	NumPadDivide,
	NumPadMultiply,
	NumPadMinus,
	NumPadPlus,
	NumPadEnter,
	NumPad1,
	NumPad2,
	NumPad3,
	NumPad4,
	NumPad5,
	NumPad6,
	NumPad7,
	NumPad8,
	NumPad9,
	NumPad0,
	NumPadPeriod,

	LeftControl = 224,
	LeftShift,
	LeftAlt,
	LeftCommand,
	RightControl,
	RightShift,
	RightAlt,
	RightCommand,

	MAX // Always leave last
};

enum class MouseButton : unsigned short
{
	LeftButton = 1,
	MiddleButton = 2,
	RightButton = 3,

	MAX
};

enum class GamepadButton : unsigned short
{
	A,
	B,
	X,
	Y,

	LB,
	RB,

	Back,
	Start,

	LeftStick,
	RightStick,

	MAX
};

enum class GamepadAxis : unsigned short
{
	LeftStickX,
	LeftStickY,

	LeftShoulder,

	RightStickX,
	RightStickY,

	RightShoulder,

	MAX
};

enum class DPadButton : unsigned short
{
	None,
	Up,
	Right,
	UpRight,
	Down,
	DownRight = 6,
	Left = 8,
	UpLeft,
	DownLeft = 12,
	MAX
};
