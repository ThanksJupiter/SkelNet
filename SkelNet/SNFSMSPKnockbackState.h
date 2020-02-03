#pragma once

#include "SNFSMState.h"

class SNFSMSPKnockbackState : public SNFSMState
{
public:
	SNFSMSPKnockbackState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float dustDelay = .15f;
};
