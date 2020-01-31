#pragma once

#include "SNFSMState.h"

struct SNFSMData;

class SNFSMAPRunState : public SNFSMState
{
public:
	SNFSMAPRunState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float dashDanceThreshold = .5f;
	float timer = 0.0f;
};
