#pragma once
#include "SNFSMState.h"
class SNFSMSPDeathState : public SNFSMState
{
public:
	SNFSMSPDeathState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
