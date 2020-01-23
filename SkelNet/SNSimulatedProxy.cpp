#include "SNSimulatedProxy.h"
#include "SNWorld.h"
#include "SNEngine.h"
#include "SNAnimator.h"

void SPDoAttack(SNWorld* world)
{
	if (world->isServer)
	{
		world->simulatedProxy.ServerCheckAttack();
	}
}

void SNSimulatedProxy::Spawn(Vector2 initPos, SNWorld& world)
{
	this->world = &world;
	position = initPos;
	animator = new SNAnimator();
	animator->SetCurrentAnimation(world.idleAnim);
	animator->defaultAnimation = world.idleAnim;
	animator->world = &world;

	if (world.isServer)
	{
		hitBox = world.SpawnHitBox(initPos, { 50, 70 }, { -25, -70 });
		attackBoxR = world.SpawnHitBox(initPos, { 30,30 }, { 100, -40 });
		attackBoxL = world.SpawnHitBox(initPos, { 30,30 }, { -130, -40 });
		hitBox->drawDebug = true;
		attackBoxR->drawDebug = true;
		attackBoxL->drawDebug = true;
	}
}

void SNSimulatedProxy::Draw(float dt)
{
	if (position.x != previousPosition.x)
	{
		if (position.x > previousPosition.x)
		{
			flip = false;
			facingRight = true;
		}
		else
		{
			flip = true;
			facingRight = false;
		}
	}

	engSetColor(0, 255, 255);

	animator->DrawAnimation(position, flip, dt);

	engSetColor(0, 0, 0);
}

void SNSimulatedProxy::ServerCheckAttack()
{
	if (!world->isServer)
		return;

	if (facingRight)
	{
		if (attackBoxR->currentState.isTriggered)
		{
			world->autonomousProxy.TakeDamage();
			world->autonomousProxy.serverWasHit = true;
		}
	}
	else
	{
		if (attackBoxL->currentState.isTriggered)
		{
			world->autonomousProxy.TakeDamage();
			world->autonomousProxy.serverWasHit = true;
		}
	}
}

void SNSimulatedProxy::PlayAttackAnim()
{
	world->spAttackAnim->AddDelegateToFrame(8, SPDoAttack);
	animator->SetCurrentAnimation(world->spAttackAnim, true);
}

void SNSimulatedProxy::TakeDamage()
{
	world->audioManager->PlayChunkOnce(world->audioManager->punch);
	printf("SimulatedProxy: Took Damage\n");
}

void SNSimulatedProxy::SetPosition(Vector2 newPosition)
{
	previousPosition = position;
	position = newPosition;

	if (world->isServer)
	{
		hitBox->UpdatePosition(position);
		attackBoxR->UpdatePosition(position);
		attackBoxL->UpdatePosition(position);
	}
}
