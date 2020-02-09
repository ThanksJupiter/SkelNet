#include "SNFSMAPWalkState.h"
#include "SNFSMData.h"
#include "SNEngine.h"
#include <cmath>
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMAPWalkState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->walkAnim);

	if (fsmData->world->HasAuthority())
	{
		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = WALK_STATE;

		fsmData->world->server.SendData(&statePacket);
	}
	else
	{
		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = WALK_STATE;

		fsmData->world->client.SendData(&statePacket);
	}

	timer = 0.0f;
}

void SNFSMAPWalkState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	timer += dt;

	if (input->leftStickDirection.x != 0)
	{
		if (abs(autoProxy->transform.GetVelocity().x) < autoProxy->minVelocitySpeed)
		{
			autoProxy->transform.SetVelocity({ autoProxy->minVelocitySpeed * input->leftStickDirection.x,autoProxy->transform.GetVelocity().y });
		}

		if (abs(autoProxy->transform.GetVelocity().x) < autoProxy->minRunSpeed)
		{
			autoProxy->transform.SetVelocity({ autoProxy->minRunSpeed * input->leftStickDirection.x, autoProxy->transform.GetAcceleration().y });
		}
	}
	else if (abs(autoProxy->transform.GetVelocity().x) < autoProxy->minVelocitySpeed)
	{
		autoProxy->EnterState(IDLE_STATE);
		return;
	}

	if (autoProxy->transform.GetFacingRight() && input->leftStickDirection.x > 0)
	{
		autoProxy->EnterState(TURNAROUND_STATE);
		return;
	}
	else if (!autoProxy->transform.GetFacingRight() && input->leftStickDirection.x < 0)
	{
		autoProxy->EnterState(TURNAROUND_STATE);
		return;
	}

	if (abs(input->leftStickDirection.x) > .8)// && timer < runTimeThreshold)
	{
		autoProxy->EnterState(RUN_STATE);
		return;
	}

	if (input->jump)
	{
		autoProxy->EnterState(JUMPSQUAT_STATE);
		return;
	}

	if (input->attack)
	{
		autoProxy->EnterState(ATTACK_STATE);
		return;
	}

	if (!autoProxy->IsGrounded())
	{
		autoProxy->EnterState(FALL_STATE);
		return;
	}

	if (input->downTaunt)
	{
		autoProxy->EnterState(TEABAG_STATE);
		return;
	}

	if (input->upTaunt)
	{
		autoProxy->EnterState(SMOKE_STATE);
		return;
	}

	if (input->rightTaunt)
	{
		autoProxy->EnterState(DOOT_STATE);
		return;
	}

	autoProxy->ForcesTimeIntegration(dt);
}

void SNFSMAPWalkState::Exit(SNFSMData* fsmData)
{

}
