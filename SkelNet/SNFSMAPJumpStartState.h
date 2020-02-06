#pragma once

#include "SNFSMState.h"

struct SNFSMData;
class SNAutonomousProxy;

class SNFSMAPJumpStartState : public SNFSMState
{
public:
	SNFSMAPJumpStartState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float animationIsDone = 420;
};
