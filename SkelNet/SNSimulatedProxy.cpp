#include "SNSimulatedProxy.h"
#include "SNWorld.h"
#include "SNEngine.h"
#include "SNAnimator.h"
#include <iomanip>
#include <sstream>

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
		//hitBox->drawDebug = true;
		//attackBoxR->drawDebug = true;
		//attackBoxL->drawDebug = true;
	}

	flyBackDirection = {-1, -1};
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

		velocity.x = position.x - previousPosition.x;
	}


	std::stringstream stream;
	stream << std::fixed << std::setprecision(3) << velocity.x;

	engDrawString({ 50, 30 }, stream.str().c_str());


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
	FlyBack();
	health += 30;
	printf("SimulatedProxy: Took Damage\n");
}

void SNSimulatedProxy::FlyBack()
{
	Vector2 newFlyback = Normalize(flyBackDirection) * (minFlyBack + health);
	//newFlyback = newFlyback * health;

	if (world->simulatedProxy.position.x < position.x)
	{
		newFlyback.x = -newFlyback.x;
	}

	position.y -= 5;

	velocity = newFlyback;
}

void SNSimulatedProxy::SetAnimation(int index)
{
	SNAnimation* newAnim;

	switch (index)
	{
		case IDLE_ANIM:
			animator->SetCurrentAnimation(world->idleAnim);
			break;

		case WALK_ANIM:
			animator->SetCurrentAnimation(world->walkAnim);
			break;

		case RUN_ANIM:
			animator->SetCurrentAnimation(world->runAnim);
			break;

		case ATTACK_ANIM:
			animator->SetCurrentAnimation(world->spAttackAnim);
			break;

		case JUMP_ANIM:
			animator->SetCurrentAnimation(world->jumpAnim);
			break;

		case KNOCKBACK_ANIM:
			animator->SetCurrentAnimation(world->knockbackAnim);
			break;

		default:
			break;
		/*case KNOCKBACK_ANIM:
			animator->SetCurrentAnimation(world->knock);
			break;*/
	}
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
