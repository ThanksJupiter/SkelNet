#pragma once

#include "SNFSMState.h"

class SNFSMSPIdleState : public SNFSMState
{
public:
	SNFSMSPIdleState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
