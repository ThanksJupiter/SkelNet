#pragma once

#include "SNFSMState.h"

class SNFSMSPJumpStartState : public SNFSMState
{
public:
	SNFSMSPJumpStartState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
