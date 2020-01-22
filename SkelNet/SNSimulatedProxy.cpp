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

bool SNSimulatedProxy::ServerCheckAttack()
{
	if (!world->isServer)
		return false;

	isAttacking = true;
	PlayAttackAnim();

	if (facingRight)
	{
		if (attackBoxR->currentState.isTriggered)
		{
			didHit = true;
		}
	}
	else
	{
		if (attackBoxL->currentState.isTriggered)
		{
			didHit = true;
		}
	}

	return didHit;
}

void SNSimulatedProxy::PlayAttackAnim()
{ // TODO: Play DoAttack(TakeDamage) after animation, Only works on client->server atm
	//world->attackAnim->AddDelegateToFrame(8, DoAttack);
	animator->SetCurrentAnimation(world->attackAnim, true);
}

void SNSimulatedProxy::DoAttack()
{
	if (ServerCheckAttack())
	{
		world->autonomousProxy.TakeDamage();
		world->autonomousProxy.serverWasHit = true;
	}
}

void SNSimulatedProxy::TakeDamage()
{
	printf("SimulatedProxy: Took Damage\n");
}

void SNSimulatedProxy::SetPosition(Vector2 newPosition)
{
	//if (isAttacking)
		//return;

	previousPosition = position;
	position = newPosition;

	if (world->isServer)
	{
		hitBox->UpdatePosition(position);
		attackBoxR->UpdatePosition(position);
		attackBoxL->UpdatePosition(position);
	}
}