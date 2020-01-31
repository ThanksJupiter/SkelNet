#pragma once

#include "SNFSMState.h"

struct SNFSMData;
class SNAutonomousProxy;

class SNFSMAPFallState : public SNFSMState
{
public:
	SNFSMAPFallState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
