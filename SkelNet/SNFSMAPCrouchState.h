#pragma once

#include "SNFSMState.h"

class SNFSMAPCrouchState : public SNFSMState
{
public:
	SNFSMAPCrouchState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
