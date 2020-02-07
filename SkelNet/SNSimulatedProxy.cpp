#include "SNWorld.h"
#include "SNSimulatedProxy.h"
#include "SNEngine.h"
#include "SNAnimator.h"
#include <iomanip>
#include <sstream>
#include "SNParticleSystem.h"
#include "SNInput.h"
#include "SNFSMData.h"
#include "SNFSMSPIdleState.h"
#include "SNFSMSPWalkState.h"
#include "SNFSMSPRunState.h"
#include "SNFSMSPAttackState.h"
#include "SNFSMSPJumpStartState.h"
#include "SNFSMSPKnockbackState.h"
#include "SNFSMSPFallState.h"
#include "SNFSMSPKnockedDownState.h"
#include "SNFSMSPTurnAroundState.h"
#include "SNFSMSPTeabagState.h"
#include "SNFSMSPJumpSquatState.h"
#include "SNFSMSPLandState.h"
#include "SNFSMSPDeathState.h"
#include "SNFSMSPJumpAscendState.h"
#include "SNFSMSPSmokeState.h"
#include "SNFSMSPDootState.h"

void SNSimulatedProxy::DoAttack()
{
	if (world->HasAuthority())
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
		attackBoxR = world.SpawnHitBox(initPos, { 100,30 }, { 40, -40 }, 0);
		attackBoxL = world.SpawnHitBox(initPos, { 100,30 }, { -140, -40 }, 0);
		hitBox->drawDebug = true;
		attackBoxR->drawDebug = true;
		attackBoxL->drawDebug = true;
	}

	InitializeFSM();
	flyBackDirection = { -1.f, -1.2f };

	currentStocks = maxStocks;
}

void SNSimulatedProxy::Update(float dt)
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

	stateMachine->Update(dt);
}

void SNSimulatedProxy::Draw(float dt, SNCamera* cam)
{
	if (transform.GetPosition().x != transform.GetPreviousPosition().x)
	{
		previousVelocity = transform.GetVelocity();
		transform.SetVelocity({ transform.GetPosition().x - transform.GetPreviousPosition().x, transform.GetVelocity().y });
	}

	engSetColor(0, 255, 255);

	animator->DrawAnimation(transform.GetPosition(), !transform.GetFacingRight(), dt);

	engSetColor(0, 0, 0);
}

void SNSimulatedProxy::ServerCheckAttack()
{
	if (!world->HasAuthority())
		return;

	SNStatePacket statePacket;
	statePacket.flag = SP_STATE_FLAG;
	statePacket.state = KNOCKBACK_STATE;

	if (transform.GetFacingRight())
	{
		if (attackBoxR->currentState.isTriggered && attackBoxR->currentState.otherId == 1)
		{
			world->server.SendData(&statePacket);
			world->autonomousProxy.SetState(KNOCKBACK_STATE);
		}
	}
	else
	{
		if (attackBoxL->currentState.isTriggered && attackBoxL->currentState.otherId == 1)
		{
			world->server.SendData(&statePacket);
			world->autonomousProxy.SetState(KNOCKBACK_STATE);
		}
	}
}

void SNSimulatedProxy::PlayAttackAnim()
{
	//world->spAttackAnim->AddDelegateToFrame(8, SPDoAttack);
	animator->SetCurrentAnimation(world->spAttackAnim, true);
}

void SNSimulatedProxy::TakeDamage()
{
	startPos = transform.GetPosition();
	startHealth = health;


	world->audioManager->PlayChunkOnce(world->audioManager->whip_hit);
	FlyBack();
	health += 15;
	printf("SimulatedProxy: Took Damage\n");

	endHealth = health;

	if (world->HasAuthority())
	{
		SNHealthPacket healthPacket;
		healthPacket.flag = AP_HEALTH_FLAG;
		healthPacket.health = health;
		healthPacket.stocks = currentStocks;

		world->server.SendData(&healthPacket);
	}
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

void SNSimulatedProxy::SetHealthAndStocks(Uint8 newHealth, Uint8 newStocks)
{
	health = newHealth;
	currentStocks = newStocks;
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

	case TAUNT_ANIM:
		animator->SetCurrentAnimation(world->teabagAnim);
		break;

	default:
		break;
	}
}

void SNSimulatedProxy::SetState(Uint8 index)
{
	const char* prevState = "none xd";
	if (stateMachine->currentState)
	{
		prevState = stateMachine->currentState->stateName;
	}

	stateMachine->EnterState(index);
}

void SNSimulatedProxy::PlayDoot(Uint8 dootFlag)
{
	switch (dootFlag)
	{
		case 0:
			world->dootAnim->sprites[9]->audio = world->audioManager->dootSound1;
			break;

		case 1:
			world->dootAnim->sprites[9]->audio = world->audioManager->dootSound2;
			break;

		case 2:
			world->dootAnim->sprites[9]->audio = world->audioManager->dootSound3;
			break;

		case 3:
			world->dootAnim->sprites[9]->audio = world->audioManager->dootSound4;
			break;
	}

	SetState(DOOT_STATE);
}

void SNSimulatedProxy::InitializeFSM()
{
	fsmData = new SNFSMData(
		world, &world->autonomousProxy, this);

	fsmData->availableStates[IDLE_STATE] = new SNFSMSPIdleState("Idle");
	fsmData->availableStates[WALK_STATE] = new SNFSMSPWalkState("Walk");
	fsmData->availableStates[RUN_STATE] = new SNFSMSPRunState("Run");
	fsmData->availableStates[ATTACK_STATE] = new SNFSMSPAttackState("Attack");
	fsmData->availableStates[JUMP_START_STATE] = new SNFSMSPJumpStartState("JumpStart");
	fsmData->availableStates[KNOCKBACK_STATE] = new SNFSMSPKnockbackState("Knockback");
	fsmData->availableStates[FALL_STATE] = new SNFSMSPFallState("Fall");
	fsmData->availableStates[KNOCKDOWN_STATE] = new SNFSMSPKnockedDownState("KnockedDown");
	fsmData->availableStates[TURNAROUND_STATE] = new SNFSMSPTurnAroundState("Turn");
	fsmData->availableStates[TEABAG_STATE] = new SNFSMSPTeabagState("Teabag");
	fsmData->availableStates[JUMPSQUAT_STATE] = new SNFSMSPJumpSquatState("JumpSquat");
	fsmData->availableStates[LAND_STATE] = new SNFSMSPLandState("Land");
	fsmData->availableStates[DEATH_STATE] = new SNFSMSPDeathState("Death");
	fsmData->availableStates[JUMP_ASCEND_STATE] = new SNFSMSPJumpAscendState("JumpAscend");
	fsmData->availableStates[SMOKE_STATE] = new SNFSMSPSmokeState("Smoke");
	fsmData->availableStates[DOOT_STATE] = new SNFSMSPDootState("Doot");

	stateMachine = new SNFiniteStateMachine(fsmData);
	fsmData->stateMachine = stateMachine;

	SetState(IDLE_STATE);
}

void SNSimulatedProxy::Reset()
{
	if (world->HasAuthority())
	{
		transform.SetPosition({ -world->spawnDistanceX, world->spawnDistanceY });

		SNTransformPacket packet;
		packet.flag = TRANSFORM_FLAG;
		packet.posX = transform.GetPosition().x;
		packet.posY = transform.GetPosition().y;
		packet.flip = transform.GetFacingRight();

		world->server.SendData(&packet);
	}
	else
	{
		transform.SetPosition({ world->spawnDistanceX, world->spawnDistanceY });
	}

	transform.SetVelocity({ 0.f, 0.f });
	transform.SetAcceleration({ 0.f, 0.f });

	if (!world->HasAuthority())
	{
		transform.SetFacingRight(true);
	}
	else
	{
		transform.SetFacingRight(false);
	}

	SetState(FALL_STATE);
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
