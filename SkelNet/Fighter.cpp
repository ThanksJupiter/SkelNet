#include "Fighter.h"
#include "World.h"
#include "Engine.h"
#include "Animator.h"

void Fighter::Spawn(Vector2 initPos, World& world)
{
	position = initPos;
	this->world = &world;
}

void Fighter::Draw()
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

	//engDrawRect(20, 60, position.x - 10, position.y - 60);
	//engSetColor(0, 0, 255);
	//engDrawRect(10, 10, position.x - 5, position.y - 5);

	engSetColor(0, 0, 0);
}

void Fighter::Update()
{
	CheckInput();
	UpdatePosition();
}

void Fighter::UpdatePosition()
{
	previousPosition = position;
	position += velocity;
	if ((velocity.x != 0 || velocity.y != 0))
	{
		world->SendTransform(position);
	}
}

void Fighter::SetPosition(Vector2 newPosition)
{
	position = newPosition;
}

void Fighter::CheckInput()
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
