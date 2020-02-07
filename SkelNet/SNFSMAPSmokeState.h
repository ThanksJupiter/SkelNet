#pragma once
#include "SNFSMState.h"
class SNFSMAPSmokeState : public SNFSMState
{
public:
	SNFSMAPSmokeState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float tauntDuration = 6 * .25;
	float pufferDelay = 8 * .25;
	bool hasSmokeSoundPlayed = false;
};