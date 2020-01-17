#pragma once

struct AnimationInstance;
class Vector2;

class SNAnimator
{
public:
	SNAnimator();

	int direction = 0;
	void SetCurrentAnimationIndex(int index);
	void DrawAnimation(Vector2 position, bool flipped);
};
