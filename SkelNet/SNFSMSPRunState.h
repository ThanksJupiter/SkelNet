#pragma once

#include "SNFSMState.h"

class SNFSMSPRunState : public SNFSMState
{
public:
	SNFSMSPRunState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
