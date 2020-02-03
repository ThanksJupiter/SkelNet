#pragma once

#include "SNFSMState.h"

class SNFSMSPJumpSquatState : public SNFSMState
{
public:
	SNFSMSPJumpSquatState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
