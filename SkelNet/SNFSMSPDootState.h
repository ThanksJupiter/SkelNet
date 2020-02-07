#pragma once
#include "SNFSMState.h"
class SNFSMSPDootState : public SNFSMState
{
public:
	SNFSMSPDootState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);
};
