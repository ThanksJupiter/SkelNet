#pragma once

#include "SNFSMState.h"

struct SNFSMData;

class SNFSMWalkState : public SNFSMState
{
public:
	SNFSMWalkState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData) override;
	void Update(float dt, SNFSMData* fsmData) override;
	void Exit(SNFSMData* fsmData) override;
};
