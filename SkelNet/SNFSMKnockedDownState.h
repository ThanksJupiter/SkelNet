#pragma once
#include "SNFSMState.h"

struct SNFSMData;
class SNAutonomousProxy;

class SNFSMKnockedDownState : public SNFSMState
{
public:
	SNFSMKnockedDownState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
