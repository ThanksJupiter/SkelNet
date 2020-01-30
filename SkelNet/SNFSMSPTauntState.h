#pragma once

#include "SNFSMState.h"

class SNFSMSPTauntState : public SNFSMState
{
public:
	SNFSMSPTauntState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
