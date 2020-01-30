#pragma once

#include "SNFSMState.h"

class SNFSMSPKnockedDownState : public SNFSMState
{
public:
	SNFSMSPKnockedDownState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
