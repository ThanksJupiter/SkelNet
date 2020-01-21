#include "SNSimulatedProxy.h"
#include "SNWorld.h"
#include "SNEngine.h"
#include "SNAnimator.h"

void SNSimulatedProxy::Spawn(Vector2 initPos, SNWorld& world)
{
	this->world = &world;
	position = initPos;
	animator = new SNAnimator();
	animator->SetCurrentAnimation(world.idleAnim);
}

void SNSimulatedProxy::Draw(float dt)
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

	animator->DrawAnimation(position, flip, dt);

	engSetColor(0, 0, 0);
}

void SNSimulatedProxy::SetPosition(Vector2 newPosition)
{
	previousPosition = position;
	position = newPosition;
}