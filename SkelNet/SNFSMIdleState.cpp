#include "SNFSMData.h"
#include "SNWorld.h"
#include "SNAutonomousProxy.h"
#include "SNAnimator.h"

void SNFSMIdleState::Enter(SNFSMData* fsmData)
{
	fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->idleAnim);
	fsmData->autonomousProxy->transform.SetVelocity({ 0, fsmData->autonomousProxy->transform.GetVelocity().y });

	if (fsmData->world->HasAuthority())
	{
		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = IDLE_STATE;

		fsmData->world->server.SendData(&statePacket);
	}
	else
	{
		SNStatePacket statePacket;
		statePacket.flag = SP_STATE_FLAG;
		statePacket.state = IDLE_STATE;

		fsmData->world->client.SendData(&statePacket);
	}
}

void SNFSMIdleState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;
	SNAutonomousProxy* autoProxy = fsmData->autonomousProxy;

	autoProxy->SetDirection();

	if (input->leftStickDirection.x != 0)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[WALK_STATE]);
		return;
	}

	if (input->jump)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[JUMP_STATE]);
		return;
	}

	if (input->attack)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[ATTACK_STATE]);
		return;
	}

	if (input->taunt)
	{
		fsmData->stateMachine->EnterState(fsmData->availableStates[TAUNT_STATE]);
		return;
	}
}

void SNFSMIdleState::Exit(SNFSMData* fsmData)
{

}
