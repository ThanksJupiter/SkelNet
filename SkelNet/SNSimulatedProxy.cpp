#include "SNSimulatedProxy.h"
#include "SNWorld.h"
#include "SNEngine.h"
#include "SNAnimator.h"
#include <iomanip>
#include <sstream>
#include "SNParticleSystem.h"

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

	transform.SetPosition(initPos);
	animator = new SNAnimator();
	animator->SetCurrentAnimation(world.idleAnim);
	animator->defaultAnimation = world.idleAnim;
	animator->world = &world;


	if (world.isServer)
	{
		hitBox = world.SpawnHitBox(initPos, { 50, 70 }, { -25, -70 }, 1);
		attackBoxR = world.SpawnHitBox(initPos, { 30,30 }, { 100, -40 }, 0);
		attackBoxL = world.SpawnHitBox(initPos, { 30,30 }, { -130, -40 }, 0);
		hitBox->drawDebug = true;
		attackBoxR->drawDebug = true;
		attackBoxL->drawDebug = true;
	}

	flyBackDirection = { -1, -1 };
}

void SNSimulatedProxy::Draw(float dt)
{
	if (transform.GetPosition().x != transform.GetPreviousPosition().x)
	{
		if (transform.GetPosition().x > transform.GetPreviousPosition().x)
		{
			flip = false;
			transform.SetFacingRight(true);
		}
		else
		{
			flip = true;
			transform.SetFacingRight(false);
		}

		previousVelocity = transform.GetVelocity();
		transform.SetVelocity({ transform.GetPosition().x - transform.GetPreviousPosition().x, transform.GetVelocity().y });
	}

	engSetColor(0, 255, 255);
	animator->DrawAnimation(transform.GetPosition(), flip, dt);
	engSetColor(0, 0, 0);
}

void SNSimulatedProxy::ServerCheckAttack()
{
	if (!world->isServer)
		return;

	if (transform.GetFacingRight())
	{
		if (attackBoxR->currentState.isTriggered && attackBoxR->currentState.otherId == 1)
		{
			world->autonomousProxy.TakeDamage();
			world->autonomousProxy.serverWasHit = true;
		}
	}
	else
	{
		if (attackBoxL->currentState.isTriggered && attackBoxL->currentState.otherId == 1)
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

	if (world->simulatedProxy.transform.GetPosition().x < transform.GetPosition().x)
	{
		newFlyback.x = -newFlyback.x;
	}

	transform.SetPosition({ transform.GetPosition().x, transform.GetPosition().y - 5 });

	transform.SetVelocity(newFlyback);
}

bool SNSimulatedProxy::isGrounded()
{
	if (transform.GetPosition().y > 332 && transform.GetPreviousPosition().y < transform.GetPosition().y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SNSimulatedProxy::SetAnimation(int index)
{
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

void SNSimulatedProxy::Reset()
{
	if (world->isServer)
	{
		transform.SetPosition({ (world->worldSize.x / 2) + 50, 0 });
	}
	else
	{
		transform.SetPosition({ (world->worldSize.x / 2) - 50, 0 });
	}

	health = 0;
	animState = 6;
	transform.SetVelocity({ 0.f, 0.f });
}

void SNSimulatedProxy::SetPosition(Vector2 newPosition)
{
	transform.SetPreviousPosition(transform.GetPosition());
	transform.SetPosition(newPosition);

	if (world->isServer)
	{
		hitBox->UpdatePosition(transform.GetPosition());
		attackBoxR->UpdatePosition(transform.GetPosition());
		attackBoxL->UpdatePosition(transform.GetPosition());
	}
}
