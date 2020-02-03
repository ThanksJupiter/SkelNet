#pragma once

#include "SNFSMState.h"

class SNFSMSPLandState : public SNFSMState
{
public:
	SNFSMSPLandState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
