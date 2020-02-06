#pragma once
#include "SNFSMState.h"
class SNFSMSPJumpAscendState : public SNFSMState
{
public:
	SNFSMSPJumpAscendState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float vortexTimer = 0.0f;
	float vortexDelay = .1f;
	float angles[4] = { 0, 90, 180, 270 };
};
