#pragma once

#include "SNFSMState.h"

class SNFSMSPKnockbackState : public SNFSMState
{
public:
	SNFSMSPKnockbackState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
