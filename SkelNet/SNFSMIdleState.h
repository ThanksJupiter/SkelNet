#pragma once

#include "SNFSMState.h"

struct SNFSMData;

class SNFSMIdleState : public SNFSMState
{
public:
	SNFSMIdleState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
