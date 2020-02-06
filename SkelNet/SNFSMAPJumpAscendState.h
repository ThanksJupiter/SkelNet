#pragma once
#include "SNFSMState.h"
class SNFSMAPJumpAscendState : public SNFSMState
{
public:
	SNFSMAPJumpAscendState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float vortexTimer = 0.0f;
	float vortexDelay = .1f;
	float maxAscendTime = 0.7f;
	float angles[4] = { 0, 90, 180, 270 };
};
