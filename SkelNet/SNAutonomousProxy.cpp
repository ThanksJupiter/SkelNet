#include "SNAutonomousProxy.h"
#include "SNWorld.h"
#include "SNEngine.h"
#include "SNAnimator.h"
#include <string>

void SNAutonomousProxy::Spawn(Vector2 initPos, SNWorld& world)
{
	position = initPos;
	this->world = &world;
	anchor.SetAbsolutePosition(position);
	canvas.Setup({ -100, -100 }, { position.x - 50.f, position.y }, &anchor);
	uiText = canvas.CreateText({50,50}, "100%");
}

void SNAutonomousProxy::Draw()
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

	std::string newText = std::to_string(uiText->anchor.GetAbsolutePosition().x);
	const char* str = newText.c_str();

	uiText->UpdateText(str);

	//anchor.SetAbsolutePosition(position);
	anchor.UpdatePosition();
	canvas.UpdatePosition();
	canvas.Draw();

	if (drawDebug)
	{
		anchor.DrawDebug(true);
	}

	engSetColor(0, 255, 0);
	animator->DrawAnimation(position, flip);
	engSetColor(0, 0, 0);
}

void SNAutonomousProxy::Update()
{
	CheckInput();
	UpdatePosition();
}

void SNAutonomousProxy::UpdatePosition()
{
	if (position.y < 333)
	{
		velocity.y += 0.001f;
	}
	else if (velocity.y > 0)
	{
		velocity.y = 0;
	}

	previousPosition = position;
	position += velocity;
	if ((velocity.x != 0 || velocity.y != 0))
	{
		world->SendTransform(position);
	}

	anchor.SetAbsolutePosition(position);
}

void SNAutonomousProxy::SetPosition(Vector2 newPosition)
{
	position = newPosition;
}

bool SNAutonomousProxy::IsGrounded()
{
	return position.y > 332;
}

void SNAutonomousProxy::CheckInput()
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

	if (engGetKeyDown(Key::Space) && IsGrounded())
	{
		velocity.y -= 0.5f;
	}

	if (engGetKeyDown(Key::S))
	{
		canvas.drawDebug = !canvas.drawDebug;
		drawDebug = !drawDebug;
	}
}


