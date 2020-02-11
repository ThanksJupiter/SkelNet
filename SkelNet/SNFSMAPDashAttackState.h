#pragma once
#include "SNFSMState.h"
class SNFSMAPDashAttackState : public SNFSMState
{
public:
	SNFSMAPDashAttackState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float attackDuration = 12 * .12f;
	float checkAttackDuration = 7 * .12f;
	bool hit = false;
	bool hasStartSoundPlayed = false;
	bool hasMissSoundPlayed = false;
	float startupSoundDelay = 2 * .12f;
	float missSoundDelay = 6 * .12f;
};