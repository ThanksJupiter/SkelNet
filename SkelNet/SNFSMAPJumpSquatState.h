#pragma once

#include "SNFSMState.h"

class SNFSMAPJumpSquatState : public SNFSMState
{
public:
	SNFSMAPJumpSquatState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float jumpDelay = .1f;
};
