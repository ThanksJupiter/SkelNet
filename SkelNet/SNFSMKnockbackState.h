#pragma once

#include "SNFSMKnockbackState.h"
#include "SNFSMState.h"

struct SNFSMData;

class SNFSMKnockbackState : public SNFSMState
{
public:
	SNFSMKnockbackState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float dustDelay = .15f;
};
