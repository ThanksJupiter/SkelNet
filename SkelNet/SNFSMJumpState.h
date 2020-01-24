#pragma once

#include "SNFSMState.h"

struct SNFSMData;
class SNAutonomousProxy;

class SNFSMJumpState : public SNFSMState
{
public:
	SNFSMJumpState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	void DoJump(SNAutonomousProxy* autoProxy);
	void LeaveLandingFrame(SNFSMData* fsmData);

	bool jumped = false;
	float jumpDelay = .1f;
	float timer = 0.0f;
};
