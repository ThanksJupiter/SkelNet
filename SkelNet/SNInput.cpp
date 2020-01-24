#include "SNInput.h"
#include "SNEngine.h"


void SNInput::SetInput()
{
	// not confusing double ternary I promise
	leftStickDirection.x = engGetKey(Key::Right) ? 1 : engGetKey(Key::Left)? -1 : 0;

	jump = engGetKeyDown(Key::Space);
	attack = engGetKeyDown(Key::X);
}
