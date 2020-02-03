#pragma once

#include "SNFSMState.h"

class SNFSMAPLandState : public SNFSMState
{
public:
	SNFSMAPLandState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float landingDelay = .1f;
};
