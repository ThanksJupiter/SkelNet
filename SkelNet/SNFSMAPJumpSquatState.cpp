#include "SNFSMAPJumpSquatState.h"
#include "SNAutonomousProxy.h"
#include "SNFSMData.h"


void SNFSMAPJumpSquatState::Enter(SNFSMData* fsmData)
{
	timer = 0.0f;
}

void SNFSMAPJumpSquatState::Update(float dt, SNFSMData* fsmData)
{
	timer += dt;

	if (timer >= jumpDelay)
	{
		fsmData->autonomousProxy->SetState(JUMP_STATE);
	}
}

void SNFSMAPJumpSquatState::Exit(SNFSMData* fsmData)
{
	
}
