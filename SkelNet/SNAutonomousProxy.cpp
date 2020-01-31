#include "SNAutonomousProxy.h"
#include "SNWorld.h"
#include "SNEngine.h"
#include "SNAnimator.h"
#include <string>
#include "SNFiniteStateMachine.h"
#include "SNFSMData.h"
#include "SNInput.h"
#include "SNFSMAPKnockedDownState.h"
#include "SNFSMAPTurnAroundState.h"
#include "SNDataPackets.h"
#include "SNFSMAPTauntState.h"
#include "SNFloor.h"

void SNAutonomousProxy::Spawn(Vector2 initPos, SNWorld& world)
{
	this->world = &world;
	transform.SetPosition(initPos);
	anchor.SetAbsolutePosition(initPos);
	canvas.Setup({ -100, -100 }, { transform.GetPosition().x - 50.f, transform.GetPosition().y }, &anchor);
	stateText = canvas.CreateText({ 250, 200 }, "100%", nullptr, { -50, 0 });

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

void SNAutonomousProxy::Draw(float dt, SNCamera* cam)
{
	if (animator->direction != 0)
	{
		if (animator->direction > 0)
		{
			transform.SetFacingRight(false);
		}
		else
		{
			transform.SetFacingRight(true);
		}
	}

	//anchor.UpdatePosition();
	//canvas.UpdatePosition();
	canvas.Draw();

	if (drawDebug)
	{
		anchor.DrawDebug(true);
	}

	//engDrawString({100, 20}, )
	stateText->UpdateText(stateMachine->currentState->stateName);

	engSetColor(0, 255, 0);

	if (animator->doManualAnimationCycling)
	{
		animator->DrawAnimation(cam->MakePositionWithCam(transform.GetPosition()), transform.GetFacingRight());
	}
	else
	{
		animator->DrawAnimation(cam->MakePositionWithCam(transform.GetPosition()), transform.GetFacingRight(), dt, animator->rotation);
	}

	engSetColor(0, 0, 0);
}

void SNAutonomousProxy::Update(float dt)
{
	CheckInput(dt);
	UpdatePosition(dt);

	//stateText->UpdateText(stateMachine->currentState->stateName);

	stateMachine->Update(dt);

	if (sendTransformToggle)
	{
		SendTransformData();
		sendTransformToggle = !sendTransformToggle;
	}
	else
	{
		sendTransformToggle = !sendTransformToggle;
	}

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
		transform.SetPosition({ 0, 0 });
	}
	else
	{
		transform.SetPosition({ 0, 0 });
	}

	health = 0;
	transform.SetVelocity({ 0.f, 0.f });
	transform.SetAcceleration({ 0.f, 0.f });
	stateMachine->EnterState(FALL_STATE);

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
			transform.SetFacingRight(false);
		}
		else
		{
			transform.SetFacingRight(true);
		}
	}
}

void SNAutonomousProxy::InitializeFSM()
{
	fsmData = new SNFSMData(
		world, this, &world->simulatedProxy, playerInput);

	fsmData->availableStates[IDLE_STATE] = new SNFSMAPIdleState("Idle");
	fsmData->availableStates[WALK_STATE] = new SNFSMAPWalkState("Walk");
	fsmData->availableStates[RUN_STATE] = new SNFSMAPRunState("Run");
	fsmData->availableStates[ATTACK_STATE] = new SNFSMAPAttackState("Attack");
	fsmData->availableStates[JUMP_STATE] = new SNFSMAPJumpState("Jump");
	fsmData->availableStates[KNOCKBACK_STATE] = new SNFSMAPKnockbackState("Knockback");
	fsmData->availableStates[FALL_STATE] = new SNFSMAPFallState("Fall");
	fsmData->availableStates[KNOCKDOWN_STATE] = new SNFSMAPKnockedDownState("KnockedDown");
	fsmData->availableStates[TURNAROUND_STATE] = new SNFSMAPTurnAroundState("Turn");
	fsmData->availableStates[TAUNT_STATE] = new SNFSMAPTauntState("Taunt");

	stateMachine = new SNFiniteStateMachine(fsmData);
	fsmData->stateMachine = stateMachine;

	EnterState(IDLE_STATE);
}

void SNAutonomousProxy::SendSPState(Uint8 state)
{
	SNStatePacket statePacket;
	statePacket.flag = SP_STATE_FLAG;
	statePacket.state = state;

	if (world->HasAuthority())
	{
		world->server.SendData(&statePacket);
	}
	else
	{
		world->client.SendData(&statePacket);
	}
}

void SNAutonomousProxy::SendAPState(Uint8 state)
{
	SNStatePacket statePacket;
	statePacket.flag = AP_STATE_FLAG;
	statePacket.state = state;

	if (world->HasAuthority())
	{
		world->server.SendData(&statePacket);
	}
	else
	{
		world->client.SendData(&statePacket);
	}
}

void SNAutonomousProxy::EnterState(Uint8 state)
{
	SendSPState(state);
	stateMachine->EnterState(state);
}

void SNAutonomousProxy::UpdatePosition(float dt)
{
	SNTransform* floorTransform = &world->worldFloor.transform;

	if (transform.GetPosition().y < floorTransform->GetPosition().y)
	{
		//Activate gravity
		transform.SetAcceleration({ transform.GetAcceleration().x ,gravity * gravityMult });
	}

	if (!IsGrounded())
	{
		//Activate gravity
		transform.SetAcceleration({ transform.GetAcceleration().x, gravity * gravityMult });

		//Set fall state
		if (transform.GetVelocity().y > 0)
		{
			if (stateMachine->currentState != fsmData->availableStates[FALL_STATE])
			{
				EnterState(FALL_STATE);
			}
		}
	}

	if (IsGrounded())
	{
		transform.SetPosition({ transform.GetPosition().x, floorTransform->GetPosition().y });
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
	transformPacket.posX = transform.GetPosition().x;
	transformPacket.posY = transform.GetPosition().y;
	transformPacket.flip = transform.GetFacingRight() ? -1 : 1;

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
	SNTransform* floorTransform = &world->worldFloor.transform;
	return (
		(transform.GetPosition().y >= floorTransform->GetPosition().y) &&
		(transform.GetPosition().x > floorTransform->GetPosition().x && transform.GetPosition().x < floorTransform->GetPosition().x + (world->levelSprite->width * floorTransform->GetScale().x))
		);
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
		statePacket.state = ATTACK_STATE;

		world->server.SendData(&statePacket);
	}
	else
	{
		//clientAttacked = true;

		animator->SetCurrentAnimation(world->apAttackAnim, true);

		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = ATTACK_STATE;

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
	if (!transform.GetFacingRight())
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

	EnterState(KNOCKBACK_STATE);
	FlyBack();

	health += 30;
	printf("AutonomousProxy: Took Damage\n");
}

void APDoAttack(SNWorld* world)
{
	world->autonomousProxy.CheckAttack();
}
