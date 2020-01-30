#pragma once

#include "SNFSMState.h"

class SNFSMSPTurnAroundState : public SNFSMState
{
public:
	SNFSMSPTurnAroundState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
