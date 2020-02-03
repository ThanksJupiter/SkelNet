#pragma once

#include "SNFSMState.h"

struct SNFSMData;
class SNAutonomousProxy;

class SNFSMAPJumpState : public SNFSMState
{
public:
	SNFSMAPJumpState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	void DoJump(SNAutonomousProxy* autoProxy);
	void LeaveLandingFrame(SNFSMData* fsmData);
};
