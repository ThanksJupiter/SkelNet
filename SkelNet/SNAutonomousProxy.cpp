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
#include "SNFSMAPTeabagState.h"
#include "SNFloor.h"
#include "SNFSMAPJumpSquatState.h"
#include "SNFSMAPLandState.h"
#include "SNFSMAPDeathState.h"
#include "SNFSMAPJumpAscendState.h"
#include "SNFSMAPSmokeState.h"

void SNAutonomousProxy::Spawn(Vector2 initPos, SNWorld& world)
{
	this->world = &world;
	transform.SetPosition(initPos);
	anchor.SetAbsolutePosition(initPos);
	canvas.Setup({ -100, -100 }, { transform.GetPosition().x - 50.f, transform.GetPosition().y }, &anchor);

	//stateText = canvas.CreateText({ 250, 200 }, "100%", 1.0f, nullptr, { -50, 0 });
	//spStateText = canvas.CreateText({ 250, 300 }, "100%", 1.0f, nullptr, { -50, 0 });
	//velocityText = canvas.CreateText({ 350, 350 }, "100%", 1.0f, nullptr, { -50, 0 });

	animator = new SNAnimator();
	animator->SetCurrentAnimation(world.idleAnim);
	animator->defaultAnimation = world.idleAnim;
	animator->world = &world;

	if (world.isServer)
	{
		hitBox = world.SpawnHitBox(initPos, { 50, 70 }, { -25, -70 }, 1);
		attackBoxR = world.SpawnHitBox(initPos, { 100, 30 }, { 40, -40 }, 0);
		attackBoxL = world.SpawnHitBox(initPos, { 100, 30 }, { -140, -40 }, 0);
		hitBox->drawDebug = true;
		attackBoxR->drawDebug = true;
		attackBoxL->drawDebug = true;
	}

	playerInput = new SNInput();
	InitializeFSM();
	flyBackDirection = { -1.f, -1.2f };

	currentStocks = maxStocks;
}

void SNAutonomousProxy::Draw(float dt, SNCamera* cam)
{
	/*if (animator->direction != 0)
	{
		if (animator->direction > 0)
		{
			transform.SetFacingRight(false);
		}
		else
		{
			transform.SetFacingRight(true);
		}
	}*/

	//anchor.UpdatePosition();
	//canvas.UpdatePosition();
	canvas.Draw();

	if (drawDebug)
	{
		anchor.DrawDebug(true);
	}

	//engDrawString({100, 20}, )
	//stateText->UpdateText(stateMachine->currentState->stateName);
	engSetTextColor(0, 255, 0);
	//spStateText->UpdateText(world->simulatedProxy.stateMachine->currentState->stateName);
	engSetTextColor(255, 255, 255);
	//velocityText->UpdateText(transform.GetVelocity().y);


	engSetColor(0, 255, 0);

	if (animator->doManualAnimationCycling)
	{
		animator->DrawAnimation(transform.GetPosition(), transform.GetFacingRight());
	}
	else
	{
		animator->DrawAnimation(transform.GetPosition(), transform.GetFacingRight(), dt, animator->rotation);
	}

	engSetColor(0, 0, 0);
}

void SNAutonomousProxy::Update(float dt)
{
	if (isInvulnerable)
	{
		timer += dt;

		if (timer >= regainVulnerabilityDelay)
		{
			isInvulnerable = false;
			timer = 0.0f;
		}
	}

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
}

void SNAutonomousProxy::ForcesTimeIntegration(float dt)
{
	transform.SetPreviousPosition(transform.GetPosition());

	transform.SetVelocity(transform.GetVelocity() + transform.GetAcceleration() * dt);
	transform.SetPosition(transform.GetPosition() + transform.GetVelocity() * dt);

	if (!IsGrounded())
	{
		transform.SetVelocity(transform.GetVelocity() * (1 - dt * drag));
	}
	else
	{
		if (abs(transform.GetVelocity().x) > 60.0f)
		{
			transform.SetVelocity(transform.GetVelocity() * (1 - dt * drag));
		}
		else
		{
			transform.SetVelocity({ 0.0f, transform.GetVelocity().y });
		}

		transform.SetPosition({ transform.GetPosition().x, world->worldFloor.transform.GetPosition().y });
		transform.SetVelocity({ transform.GetVelocity().x, 0 });
	}
}

void SNAutonomousProxy::FlyBack()
{
	Vector2 newFlyback = Normalize(flyBackDirection) * (minFlyBack + health);

	if (world->simulatedProxy.transform.GetPosition().x < transform.GetPosition().x)
	{
		newFlyback.x = -newFlyback.x;
		transform.SetFacingRight(true);
	}
	else
	{
		transform.SetFacingRight(false);
	}

	transform.SetPosition({ transform.GetPosition().x, transform.GetPosition().y - 5 });
	transform.SetVelocity(newFlyback);
}

void SNAutonomousProxy::Reset()
{
	if (world->HasAuthority())
	{
		transform.SetPosition({ world->spawnDistanceX, world->spawnDistanceY });
	}
	else
	{
		transform.SetPosition({ -world->spawnDistanceX, world->spawnDistanceY });
	}

	transform.SetVelocity({ 0.f, 0.f });
	transform.SetAcceleration({ 0.f, 0.f });

	if (world->HasAuthority())
	{
		transform.SetFacingRight(true);
	}
	else
	{
		transform.SetFacingRight(false);
	}

	SetState(FALL_STATE);
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
	fsmData->availableStates[JUMP_START_STATE] = new SNFSMAPJumpStartState("JumpStart");
	fsmData->availableStates[KNOCKBACK_STATE] = new SNFSMAPKnockbackState("Knockback");
	fsmData->availableStates[FALL_STATE] = new SNFSMAPFallState("Fall");
	fsmData->availableStates[KNOCKDOWN_STATE] = new SNFSMAPKnockedDownState("KnockedDown");
	fsmData->availableStates[TURNAROUND_STATE] = new SNFSMAPTurnAroundState("Turn");
	fsmData->availableStates[TEABAG_STATE] = new SNFSMAPTeabagState("Teabag");
	fsmData->availableStates[JUMPSQUAT_STATE] = new SNFSMAPJumpSquatState("JumpSquat");
	fsmData->availableStates[LAND_STATE] = new SNFSMAPLandState("Land");
	fsmData->availableStates[DEATH_STATE] = new SNFSMAPDeathState("Death");
	fsmData->availableStates[JUMP_ASCEND_STATE] = new SNFSMAPJumpAscendState("JumpAscend");
	fsmData->availableStates[SMOKE_STATE] = new SNFSMAPSmokeState("Smoke");

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
	const char* prevState = "none xd";
	if (stateMachine->currentState)
	{
		prevState = stateMachine->currentState->stateName;
	}

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
		/*if (transform.GetVelocity().y > 0)
		{
			if (stateMachine->currentState != fsmData->availableStates[FALL_STATE])
			{
				EnterState(FALL_STATE);
			}
		}*/
	}

	/*if (IsGrounded())
	{
		transform.SetPosition({ transform.GetPosition().x, floorTransform->GetPosition().y });
		transform.SetVelocity({ transform.GetVelocity().x, 0 });
	}*/

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

	bool atFloor = false;
	// HACK: y-position was -0.tiny number, seems to work with       - 0.1 here
	if (transform.GetPosition().y >= floorTransform->GetPosition().y - 0.1)
	{
		atFloor = true;
	}

	bool withinLevel = (transform.GetPosition().x > floorTransform->GetPosition().x && transform.GetPosition().x < floorTransform->GetPosition().x + (world->levelSprite->width * floorTransform->GetScale().x));

	bool retVal = atFloor && withinLevel;
	return retVal;
}

void SNAutonomousProxy::CheckInput(float dt)
{
	if (playerInput->restart)
	{
		world->RespawnPlayerEvent();
	}

	if (inputEnabled)
	{
		playerInput->SetInput();

		if (engGetKeyDown(Key::S))
		{
			canvas.drawDebug = !canvas.drawDebug;
			drawDebug = !drawDebug;
		}
	}
}

void SNAutonomousProxy::SendEnterAttackState()
{
	/* if Server */
	// play attack anim
	// check if hit simulated proxy
	// send if hit to client

	if (world->HasAuthority())
	{
		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = ATTACK_STATE;
		world->server.SendData(&statePacket);
	}
	else
	{
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
	SNStatePacket statePacket;
	statePacket.flag = AP_STATE_FLAG;
	statePacket.state = KNOCKBACK_STATE;

	if (!transform.GetFacingRight())
	{
		if (attackBoxR->currentState.isTriggered && attackBoxR->currentState.otherId == 1)
		{
			world->server.SendData(&statePacket);
			world->simulatedProxy.SetState(KNOCKBACK_STATE);
		}
	}
	else
	{
		if (attackBoxL->currentState.isTriggered && attackBoxL->currentState.otherId == 1)
		{
			world->server.SendData(&statePacket);
			world->simulatedProxy.SetState(KNOCKBACK_STATE);
		}
	}
}

void SNAutonomousProxy::TakeDamage()
{
	world->audioManager->PlayChunkOnce(world->audioManager->whip_hit);
	FlyBack();
	health += 15;
	printf("AutonomousProxy: Took Damage\n");
}

void SNAutonomousProxy::DoAttack()
{
	world->autonomousProxy.CheckAttack();
}

void SNAutonomousProxy::SetState(Uint8 index)
{
	// HACK this shit might be needed but it doesn't seem like it
	//if (stateMachine->currentStateIndex == DEATH_STATE)
	//{
		//printf("[SetState] tried to enter state: '%s' while dead, this is illegal and causes nought but trouble\n", fsmData->availableStates[index]->stateName);
		//return;
	//}

	const char* prevState = "none xd";
	if (stateMachine->currentState)
	{
		prevState = stateMachine->currentState->stateName;
	}

	stateMachine->EnterState(index);
}