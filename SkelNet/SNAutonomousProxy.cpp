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
	uiText = canvas.CreateText({ -50, -100 }, "100%", nullptr, {-50, 0});
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

	uiText->UpdateText(position.y);

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
	SendData();
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
	//if ((velocity.x != 0 || velocity.y != 0))
	//{
	//	world->SendPlayerData(position, health);
	//}
	//
	anchor.SetAbsolutePosition(position);
}

void SNAutonomousProxy::SendData()
{
	world->SendPlayerData(position, health);
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


