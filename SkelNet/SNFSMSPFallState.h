#pragma once
#include "SNFSMState.h"

class SNFSMSPFallState : public SNFSMState
{
public:
	SNFSMSPFallState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
