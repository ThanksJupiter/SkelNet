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

	//uiText = canvas.CreateText({ -50, -100 }, "100%", nullptr, {-50, 0});
	accText = canvas.CreateText({ 50, -100 }, "100%", nullptr, { -50, 0 });
	velText = canvas.CreateText({ -50, -100 }, "100%", nullptr, { -50, 0 });

	animator = new SNAnimator();
	animator->SetCurrentAnimation(world.idleAnim);
	animator->defaultAnimation = world.idleAnim;

	if (world.isServer)
	{
		hitBox = world.SpawnHitBox(initPos, { 50, 70 }, { -25, -70 });
		attackBoxR = world.SpawnHitBox(initPos, { 30,30 }, { 110, -40 });
		attackBoxL = world.SpawnHitBox(initPos, { 30,30 }, { -140, -40 });
		hitBox->drawDebug = true;
		attackBoxR->drawDebug = true;
		attackBoxL->drawDebug = true;
	}

	//world.attackAnim->AddDelegateToFrame(8, Attack);
}

void SNAutonomousProxy::Draw(float dt)
{
	if (animator->direction != 0)
	{
		if (animator->direction > 0)
		{
			flip = false;
		}
		else
		{
			flip = true;
		}
	}

	//uiText->UpdateText(position.y);
	accText->UpdateText(acceleration.x);
	velText->UpdateText(velocity.x);

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

void SNAutonomousProxy::Update(float dt)
{
	CheckInput(dt);
	UpdatePosition(dt);

	SendData();

	serverAttacked = false;
	clientAttacked = false;
	clientWasHit = false;
	serverWasHit = false;
}

void SNAutonomousProxy::UpdatePosition(float dt)
{
	previousPosition = position;

	velocity += acceleration * dt;
	position += velocity * dt;

	if (position.y < 333)
	{
		acceleration.y = gravity * gravityMult;
	}

	if (velocity.y > 0 && position.y > 333)
	{
		position.y = 333;
		velocity.y = 0;
	}

	if (world->isServer)
	{
		hitBox->UpdatePosition(position);
		attackBoxR->UpdatePosition(position);
		attackBoxL->UpdatePosition(position);
	}
}

void SNAutonomousProxy::SendData()
{
	world->SendPlayerData(position, health, serverAttacked, serverWasHit, clientAttacked, clientWasHit);
}

void SNAutonomousProxy::SetPosition(Vector2 newPosition)
{
	position = newPosition;
}

bool SNAutonomousProxy::IsGrounded()
{
	return position.y > 332;
}

void SNAutonomousProxy::CheckInput(float dt)
{
	if (!animator->movementLocked)
	{
		if (engGetKey(Key::Left))
		{
			if (!animator->isWalking && !animator->isRunning)
			{
				animator->SetCurrentAnimation(world->walkAnim);
				animator->isWalking = true;
			}

			if (velocity.x > -minVelocitySpeed && IsGrounded())
			{
				animator->isRunning = false;
				velocity.x = -minVelocitySpeed;
			}

			if (velocity.x > -maxVelocitySpeed)
			{
				acceleration.x = -accelerationSpeed;
				if (!animator->isRunning && velocity.x < -minRunSpeed)
				{
					animator->SetCurrentAnimation(world->runAnim);
					animator->isRunning = true;
					animator->isWalking = false;
				}
			}
			else
			{
				acceleration.x = 0.0f;
			}

			animator->direction = -1;
			facingRight = false;
		}
		else if (engGetKey(Key::Right))
		{
			if (!animator->isWalking && !animator->isRunning)
			{
				animator->SetCurrentAnimation(world->walkAnim);
				animator->isWalking = true;
			}

			if (velocity.x < minVelocitySpeed && IsGrounded())
			{
				animator->isRunning = false;
				velocity.x = minVelocitySpeed;
			}

			if (velocity.x < maxVelocitySpeed)
			{
				acceleration.x = accelerationSpeed;
				if (!animator->isRunning && velocity.x > minRunSpeed)
				{
					animator->SetCurrentAnimation(world->runAnim);
					animator->isRunning = true;
					animator->isWalking = false;
				}
			}
			else
			{
				acceleration.x = 0.0f;
			}

			animator->direction = 1;
			facingRight = true;
		}
		else {
			if (animator->isWalking || animator->isRunning)
			{
				animator->SetCurrentAnimation(world->idleAnim);
				animator->isWalking = false;
			}

			if (IsGrounded())
			{
				velocity.x = 0;
				animator->direction = 0;
			}

			acceleration.x = 0;
		}
	}

	if (!IsGrounded())
	{
		if (engGetKey(Key::Left))
		{
			animator->direction = -1;
		}

		if (engGetKey(Key::Right))
		{
			animator->direction = 1;
		}
	}

	if (engGetKeyDown(Key::Space) && IsGrounded() && !animator->movementLocked)
	{
		acceleration.x = 0.0f;
		velocity.y = -200.0f;
	}

	if (engGetKeyDown(Key::X) && IsGrounded() && !animator->movementLocked)
	{
		Attack();
	}

	if (engGetKeyDown(Key::S))
	{
		canvas.drawDebug = !canvas.drawDebug;
		drawDebug = !drawDebug;
	}
}

void SNAutonomousProxy::Attack()
{
	/* if Server */
	// play attack anim
	// check if hit simulated proxy
	// send if hit to client

	if (world->isServer)
	{
		animator->movementLocked = true;
		animator->isWalking = false;
		animator->isRunning = false;
		animator->SetCurrentAnimation(world->attackAnim, true);
		velocity.x = 0.0f;
		acceleration.x = 0.0f;
		animator->direction = 0;

		serverAttacked = true;

		if (facingRight)
		{
			if (attackBoxR->currentState.isTriggered)
			{
				// Send hit data
				clientWasHit = true;
				world->simulatedProxy.PlayAttackAnim();
			}
		}
		else
		{
			if (attackBoxL->currentState.isTriggered)
			{
				// Send hit data
				clientWasHit = true;
				world->simulatedProxy.PlayAttackAnim();
			}
		}
	}
	else
	{
		animator->movementLocked = true;
		animator->isWalking = false;
		animator->SetCurrentAnimation(world->attackAnim, true);
		velocity.x = 0.0f;
		animator->direction = 0;

		clientAttacked = true;
	}

	/* if Client*/
	// play attack anim
	// send attack to server
	// get if attack hit from server
	// call TakeDamage on simulated proxy
}

void SNAutonomousProxy::TakeDamage()
{
	printf("AutonomousProxy: Took Damage\n");
}
