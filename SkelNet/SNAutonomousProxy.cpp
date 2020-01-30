#include "SNAutonomousProxy.h"
#include "SNWorld.h"
#include "SNEngine.h"
#include "SNAnimator.h"
#include <string>
#include "SNFiniteStateMachine.h"
#include "SNFSMData.h"
#include "SNInput.h"
#include "SNFSMKnockedDownState.h"
#include "SNFSMTurnAroundState.h"
#include "SNDataPackets.h"
#include "SNFSMAPTauntState.h"

void SNAutonomousProxy::Spawn(Vector2 initPos, SNWorld& world)
{
	transform.SetPosition(initPos);
	this->world = &world;
	anchor.SetAbsolutePosition(initPos);
	canvas.Setup({ -100, -100 }, { transform.GetPosition().x - 50.f, transform.GetPosition().y }, &anchor);
	stateText = canvas.CreateText({ 0, -200 }, "100%", nullptr, { -50, 0 });

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
		animator->DrawAnimation(transform.GetPosition(), flip);
	}
	else
	{
		animator->DrawAnimation(transform.GetPosition(), flip, dt, animator->rotation);
	}

	engSetColor(0, 0, 0);
}

void SNAutonomousProxy::Update(float dt)
{
	CheckInput(dt);
	UpdatePosition(dt);

	stateText->UpdateText(stateMachine->currentState->stateName);

	stateMachine->Update(dt);

	//TODO: add SendTransformData() to all moving states

	serverAttacked = false;
	clientAttacked = false;
	clientWasHit = false;
	serverWasHit = false;
}

void SNAutonomousProxy::FlyBack()
{
	Vector2 newFlyback = Normalize(flyBackDirection) * (minFlyBack + health);

	if (world->simulatedProxy.transform.GetPosition().x < transform.GetPosition().x)
	{
		newFlyback.x = -newFlyback.x;
	}
	transform.SetPosition({ transform.GetPosition().x, transform.GetPosition().y - 5 });
	transform.SetVelocity(newFlyback);
}

void SNAutonomousProxy::Reset()
{
	if (world->isServer)
	{
		transform.SetPosition({ (world->worldSize.x / 2) - 50, 0 });
	}
	else
	{
		transform.SetPosition({ (world->worldSize.x / 2) + 50, 0 });
	}

	health = 0;
	transform.SetVelocity({ 0.f, 0.f });
	transform.SetAcceleration({ 0.f, 0.f });
	stateMachine->EnterState(fsmData->availableStates[FALL_STATE]);

	serverAttacked = false;
	serverWasHit = false;
	clientAttacked = false;
	clientWasHit = false;
}

void SNAutonomousProxy::SetDirection()
{
	if (abs(playerInput->leftStickDirection.x) > .1f)
	{
		Vector2 normStickDir = Normalize(playerInput->leftStickDirection);
		animator->direction = normStickDir.x;
	}

	if (engGetKey(Key::Left))
	{
		animator->direction = -1;
	}

	if (engGetKey(Key::Right))
	{
		animator->direction = 1;
	}

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
	fsmData->availableStates[KNOCKDOWN_STATE] = new SNFSMKnockedDownState("KnockedDown");
	fsmData->availableStates[TURNAROUND_STATE] = new SNFSMTurnAroundState("Turn");
	fsmData->availableStates[TAUNT_STATE] = new SNFSMAPTauntState("Taunt");

	stateMachine = new SNFiniteStateMachine(fsmData);
	fsmData->stateMachine = stateMachine;

	stateMachine->EnterState(fsmData->availableStates[0]);
}

void SNAutonomousProxy::UpdatePosition(float dt)
{
	if (transform.GetPosition().y < 333)
	{
		//Activate gravity
		transform.SetAcceleration({ transform.GetAcceleration().x ,gravity * gravityMult });
	}

	if ((transform.GetPosition().x < 170 || transform.GetPosition().x > 935))
	{
		//Activate gravity
		transform.SetAcceleration({ transform.GetAcceleration().x ,gravity * gravityMult });

		//Set fall state
		if (stateMachine->currentState != fsmData->availableStates[FALL_STATE])
		{
			stateMachine->EnterState(fsmData->availableStates[FALL_STATE]);
		}
	}

	if ((transform.GetVelocity().y > 0 && transform.GetPosition().y > 333) && ((transform.GetPosition().x > 170 && transform.GetPosition().x < 935)))
	{
		transform.SetPosition({ transform.GetPosition().x, 333 });
		transform.SetVelocity({ transform.GetVelocity().x, 0 });
	}

	if (world->isServer)
	{
		hitBox->UpdatePosition(transform.GetPosition());
		attackBoxR->UpdatePosition(transform.GetPosition());
		attackBoxL->UpdatePosition(transform.GetPosition());
	}
}

void SNAutonomousProxy::SendTransformData()
{
	SNTransformPacket transformPacket;
	transformPacket.flag = TRANSFORM_FLAG;
	transformPacket.posX = position.x;
	transformPacket.posY = position.y;
	transformPacket.flip = flip ? -1 : 1;

	if (world->HasAuthority())
	{
		world->server.SendData(&transformPacket);
	}
	else
	{
		world->client.SendData(&transformPacket);
	}
}

void SNAutonomousProxy::SetPosition(Vector2 newPosition)
{
	transform.SetPosition(newPosition);
}

bool SNAutonomousProxy::IsGrounded()
{
	return transform.GetPosition().y > 332;
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
		//serverAttacked = true;


		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = KNOCKDOWN_STATE;

		world->server.SendData(&statePacket);
	}
	else
	{
		//clientAttacked = true;

		animator->SetCurrentAnimation(world->apAttackAnim, true);

		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = KNOCKDOWN_STATE;

		world->client.SendData(&statePacket);
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
			//clientWasHit = true;
			world->simulatedProxy.TakeDamage();
		}
	}
	else
	{
		if (attackBoxL->currentState.isTriggered && attackBoxL->currentState.otherId == 1)
		{
			// Send hit data
			//clientWasHit = true;
			world->simulatedProxy.TakeDamage();
		}
	}
}

void SNAutonomousProxy::TakeDamage()
{
	world->audioManager->PlayChunkOnce(world->audioManager->whip_hit);

	stateMachine->EnterState(fsmData->availableStates[KNOCKBACK_STATE]);
	FlyBack();

	health += 30;
	printf("AutonomousProxy: Took Damage\n");
}

void APDoAttack(SNWorld* world)
{
	world->autonomousProxy.CheckAttack();
}
