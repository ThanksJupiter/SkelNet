#pragma once

#include "SNFSMState.h"

class SNFSMSPWalkState : public SNFSMState
{
public:
	SNFSMSPWalkState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
