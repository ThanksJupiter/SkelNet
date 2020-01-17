#include "AutonomousProxy.h"
#include "World.h"
#include "Engine.h"
#include "Animator.h"

void AutonomousProxy::Spawn(Vector2 initPos, World& world)
{
	position = initPos;
	this->world = &world;
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

	engSetColor(0, 255, 0);
	animator->DrawAnimation(position, flip);

	engSetColor(0, 0, 0);
}

void AutonomousProxy::Update()
{
	CheckInput();
	UpdatePosition();
}

void AutonomousProxy::UpdatePosition()
{
	previousPosition = position;
	position += velocity;
	if ((velocity.x != 0 || velocity.y != 0))
	{
		world->SendTransform(position);
	}
}

void AutonomousProxy::SetPosition(Vector2 newPosition)
{
	position = newPosition;
}

void AutonomousProxy::CheckInput()
{
	if (engGetKey(Key::Left))
	{
		velocity.x = -0.5f;
		animator->direction = -1;
	}
	else if (engGetKey(Key::Right))
	{
		velocity.x = 0.5f;
		animator->direction = 1;
	}
	else {
		velocity.x = 0.0f;
		animator->direction = 0;
	}

	if (engGetKey(Key::Space))
	{
		velocity.y -= 0.001f;
	}
}
