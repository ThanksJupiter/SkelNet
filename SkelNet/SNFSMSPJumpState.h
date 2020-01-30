#pragma once

#include "SNFSMState.h"

class SNFSMSPJumpState : public SNFSMState
{
public:
	SNFSMSPJumpState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
