#include "SNAutonomousProxy.h"
#include "SNWorld.h"
#include "SNEngine.h"
#include "SNAnimator.h"
#include <string>

void SNAutonomousProxy::Spawn(Vector2 initPos, SNWorld& world)
{
	position = initPos;
	this->world = &world;
	anchor.SetAbsolutePosition(initPos);
	canvas.Setup({ -100, -100 }, { position.x - 50.f, position.y }, &anchor);
	uiText = canvas.CreateText({ -50, -100 }, "100%", nullptr, {-50, 0});

	hitBox = world.SpawnHitBox(initPos, { 50, 70 }, { -25, -70} );
	attackBoxR = world.SpawnHitBox(initPos, { 30,30 }, { 110, -40 });
	attackBoxL = world.SpawnHitBox(initPos, { 30,30 }, { -110, -40 });
	hitBox->drawDebug = true;
	attackBoxR->drawDebug = true;
	attackBoxL->drawDebug = true;

	animator = new SNAnimator();
	animator->SetCurrentAnimation(world.idleAnim);
}

void SNAutonomousProxy::Draw(float dt)
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
	animator->DrawAnimation(position, flip, dt);
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

	anchor.SetAbsolutePosition(position);
	hitBox->UpdatePosition(position);
	attackBoxR->UpdatePosition(position);
	attackBoxL->UpdatePosition(position);
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
	if (!animator->movementLocked)
	{
		if (engGetKey(Key::Left))
		{
			if (!animator->isWalking)
			{
				animator->SetCurrentAnimation(world->walkAnim);
				animator->isWalking = true;
			}
			velocity.x = -0.3f;
			animator->direction = -1;
		}
		else if (engGetKey(Key::Right))
		{
			if (!animator->isWalking)
			{
				animator->SetCurrentAnimation(world->walkAnim);
				animator->isWalking = true;
			}
			velocity.x = 0.3f;
			animator->direction = 1;
		}
		else {
			if (animator->isWalking)
			{
				animator->SetCurrentAnimation(world->idleAnim);
				animator->isWalking = false;
			}
			velocity.x = 0.0f;
			animator->direction = 0;
		}
	}

	if (engGetKeyDown(Key::Space) && IsGrounded() && !animator->movementLocked)
	{
		velocity.y -= 0.5f;
	}

	if (engGetKeyDown(Key::X) && IsGrounded() && !animator->movementLocked)
	{
		animator->movementLocked = true;
		animator->isWalking = false;
		animator->SetCurrentAnimation(world->attackAnim, true);
		velocity.x = 0.0f;
		animator->direction = 0;
	}

	if (engGetKeyDown(Key::S))
	{
		canvas.drawDebug = !canvas.drawDebug;
		drawDebug = !drawDebug;
	}
}
