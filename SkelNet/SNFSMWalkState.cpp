#include "SNFSMWalkState.h"
#include "SNFSMData.h"
#include "SNEngine.h"
#include <cmath>
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMWalkState::Enter(SNFSMData* fsmData)
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

void SNFSMWalkState::Update(float dt, SNFSMData* fsmData)
{
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;
	SNInput* input = fsmData->input;

	timer += dt;

	autoProxy->SetDirection();

	if (input->leftStickDirection.x != 0)
	{
		if (abs(autoProxy->transform.GetVelocity().x) < autoProxy->minVelocitySpeed)
		{
			autoProxy->transform.SetVelocity({ autoProxy->minVelocitySpeed * input->leftStickDirection.x,autoProxy->transform.GetVelocity().y });
		}

		if (abs(autoProxy->transform.GetVelocity().x) < autoProxy->minRunSpeed)
		{
			autoProxy->transform.SetAcceleration({ autoProxy->accelerationSpeed * input->leftStickDirection.x, autoProxy->transform.GetAcceleration().y });
		}
	}
	else
	{
		// TODO don't leave walk state if no input
		autoProxy->transform.SetVelocity({ 0.0f, autoProxy->transform.GetVelocity().y});
		autoProxy->transform.SetAcceleration({ 0.0f, autoProxy->transform.GetAcceleration().y });

		autoProxy->EnterState(IDLE_STATE);
		return;
	}

	if (abs(input->leftStickDirection.x) > .8 && timer < runTimeThreshold)
	{
		autoProxy->EnterState(RUN_STATE);
		return;
	}

	if (input->jump)
	{
		autoProxy->EnterState(JUMP_STATE);
		return;
	}

	if (input->attack)
	{
		autoProxy->EnterState(ATTACK_STATE);
		return;
	}

	// movement time integration
	autoProxy->transform.SetPreviousPosition(autoProxy->transform.GetPosition());

	autoProxy->transform.SetVelocity(autoProxy->transform.GetVelocity() + autoProxy->transform.GetAcceleration() * dt);
	autoProxy->transform.SetPosition(autoProxy->transform.GetPosition() + autoProxy->transform.GetVelocity() * dt);

	autoProxy->SendTransformData();
}

void SNFSMWalkState::Exit(SNFSMData* fsmData)
{

}
