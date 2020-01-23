#pragma once

#include "SNFSMState.h"

struct SNFSMData;

class SNFSMJumpState : public SNFSMState
{
public:
	SNFSMJumpState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
