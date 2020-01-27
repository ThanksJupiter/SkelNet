#include "SNAutonomousProxy.h"
#include "SNWorld.h"
#include "SNEngine.h"
#include "SNAnimator.h"
#include <string>
#include "SNFiniteStateMachine.h"
#include "SNFSMData.h"
#include "SNInput.h"

void SNAutonomousProxy::Spawn(Vector2 initPos, SNWorld& world)
{
	position = initPos;
	this->world = &world;
	anchor.SetAbsolutePosition(initPos);
	canvas.Setup({ -100, -100 }, { position.x - 50.f, position.y }, &anchor);
	stateText = canvas.CreateText({ 0, -200 }, "100%", nullptr, { -50, 0 });

	animator = new SNAnimator();
	animator->SetCurrentAnimation(world.idleAnim);
	animator->defaultAnimation = world.idleAnim;
	animator->world = &world;

	if (world.isServer)
	{
		hitBox = world.SpawnHitBox(initPos, { 50, 70 }, { -25, -70 }, 1);
		attackBoxR = world.SpawnHitBox(initPos, { 30,30 }, { 100, -40 });
		attackBoxL = world.SpawnHitBox(initPos, { 30,30 }, { -130, -40 });
		//hitBox->drawDebug = true;
		//attackBoxR->drawDebug = true;
		//attackBoxL->drawDebug = true;
	}

	playerInput = new SNInput();
	InitializeFSM();
	flyBackDirection = { -1, -1 };
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

	anchor.UpdatePosition();
	canvas.UpdatePosition();
	canvas.Draw();

	if (drawDebug)
	{
		anchor.DrawDebug(true);
	}

	engSetColor(0, 255, 0);

	if (animator->doManualAnimationCycling)
	{
		animator->DrawAnimation(position, flip);
	}
	else
	{
		animator->DrawAnimation(position, flip, dt, animator->rotation);
	}

	engSetColor(0, 0, 0);
}

void SNAutonomousProxy::Update(float dt)
{
	CheckInput(dt);
	UpdatePosition(dt);

	stateText->UpdateText(stateMachine->currentState->stateName);

	stateMachine->Update(dt);

	SendData();

	serverAttacked = false;
	clientAttacked = false;
	clientWasHit = false;
	serverWasHit = false;
}

void SNAutonomousProxy::FlyBack()
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

void SNAutonomousProxy::SetDirection()
{
	animator->direction = playerInput->leftStickDirection.x;

	if (engGetKey(Key::Left))
	{
		animator->direction = -1;
	}

	if (engGetKey(Key::Right))
	{
		animator->direction = 1;
	}
}

void SNAutonomousProxy::InitializeFSM()
{
	fsmData = new SNFSMData(
		world, this, &world->simulatedProxy, playerInput);

	fsmData->availableStates[IDLE_STATE] = new SNFSMIdleState("Idle");
	fsmData->availableStates[WALK_STATE] = new SNFSMWalkState("Walk");
	fsmData->availableStates[RUN_STATE] = new SNFSMRunState("Run");
	fsmData->availableStates[ATTACK_STATE] = new SNFSMAttackState("Attack");
	fsmData->availableStates[JUMP_STATE] = new SNFSMJumpState("Jump");
	fsmData->availableStates[KNOCKBACK_STATE] = new SNFSMKnockbackState("Knockback");
	fsmData->availableStates[FALL_STATE] = new SNFSMFallState("Fall");

	stateMachine = new SNFiniteStateMachine(fsmData);
	fsmData->stateMachine = stateMachine;

	stateMachine->EnterState(fsmData->availableStates[0]);
}

void SNAutonomousProxy::UpdatePosition(float dt)
{
	if (position.y < 333)
	{
		//Activate gravity
		acceleration.y = gravity * gravityMult;
	}
	
	if ((position.x < 170 || position.x > 935))
	{
		//Activate gravity
		acceleration.y = gravity * gravityMult;

		//Set fall state
		if (stateMachine->currentState != stateMachine->availableStates[FALL_STATE])
		{
			stateMachine->EnterState(fsmData->availableStates[FALL_STATE]);
		}
	}

	if ((velocity.y > 0 && position.y > 333) && ((position.x > 170 && position.x < 935)))
	{
		{
			position.y = 333;
			velocity.y = 0;
		}
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
	playerInput->SetInput();

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
		serverAttacked = true;
	}
	else
	{
		clientAttacked = true;

		animator->SetCurrentAnimation(world->apAttackAnim, true);
	}

	/* if Client*/
	// play attack anim
	// send attack to server
	// get if attack hit from server
	// call TakeDamage on simulated proxy
}

void SNAutonomousProxy::CheckAttack()
{
	if (!flip)
	{
		if (attackBoxR->currentState.isTriggered && attackBoxR->currentState.otherId == 1)
		{
			// Send hit data
			clientWasHit = true;
			world->simulatedProxy.TakeDamage();
		}
	}
	else
	{
		if (attackBoxL->currentState.isTriggered && attackBoxR->currentState.otherId == 1)
		{
			// Send hit data
			clientWasHit = true;
			world->simulatedProxy.TakeDamage();
		}
	}
}

void SNAutonomousProxy::TakeDamage()
{
	world->audioManager->PlayChunkOnce(world->audioManager->punch);

	stateMachine->EnterState(fsmData->availableStates[KNOCKBACK_STATE]);
	FlyBack();

	health += 30;
	printf("AutonomousProxy: Took Damage\n");
}

void APDoAttack(SNWorld* world)
{
	world->autonomousProxy.CheckAttack();
}
