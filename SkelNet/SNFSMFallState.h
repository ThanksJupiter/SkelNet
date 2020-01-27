#pragma once

#include "SNFSMState.h"

struct SNFSMData;
class SNAutonomousProxy;

class SNFSMFallState : public SNFSMState
{
public:
	SNFSMFallState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
