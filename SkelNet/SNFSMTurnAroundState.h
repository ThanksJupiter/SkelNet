#pragma once

#include "SNFSMState.h"

class SNFSMTurnAroundState : public SNFSMState
{
public:
	SNFSMTurnAroundState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
