#pragma once
#include "SNFSMState.h"
class SNFSMAPDeathState : public SNFSMState
{
public:
	SNFSMAPDeathState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
