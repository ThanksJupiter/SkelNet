#pragma once

#include "SNFSMState.h"

struct SNFSMData;

class SNFSMSPAttackState : public SNFSMState
{
public:
	SNFSMSPAttackState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float attackDuration = 12 * .12f;
	float checkAttackDuration = 7 * .12f;
	bool hit = false;
};
