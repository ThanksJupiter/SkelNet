#include "AutonomousProxy.h"
#include "World.h"
#include "Engine.h"
#include "Animator.h"

void AutonomousProxy::Spawn(Vector2 initPos)
{
	position = initPos;
}

void AutonomousProxy::Draw()
{
	if (position.x != previousPosition.x)
	{
		if (position.x > previousPosition.x)
		{
			flip = false;
		}
		else
		{
			flip = true;
		}
	}

	engSetColor(0, 255, 255);

	animator->DrawAnimation(position, flip);

	engSetColor(0, 0, 0);
}

void AutonomousProxy::SetPosition(Vector2 newPosition)
{
	previousPosition = position;
	position = newPosition;
}