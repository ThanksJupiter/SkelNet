#pragma once

#include "SNFSMState.h"

struct SNFSMData;

class SNFSMAPWalkState : public SNFSMState
{
public:
	SNFSMAPWalkState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData) override;
	void Update(float dt, SNFSMData* fsmData) override;
	void Exit(SNFSMData* fsmData) override;

	float runTimeThreshold = .05f;
	float timer = 0.0f;
};
