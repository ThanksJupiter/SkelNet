#pragma once

#include "SNFSMState.h"

struct SNFSMData;

class SNFSMAttackState : public SNFSMState
{
public:
	SNFSMAttackState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float attackDuration = 12 * .15f;
};

