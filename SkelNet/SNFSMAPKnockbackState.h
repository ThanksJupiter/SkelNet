#pragma once

#include "SNFSMAPKnockbackState.h"
#include "SNFSMState.h"

struct SNFSMData;

class SNFSMAPKnockbackState : public SNFSMState
{
public:
	SNFSMAPKnockbackState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float dustDelay = .15f;
};
