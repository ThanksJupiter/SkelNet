#pragma once

#include "SNFSMState.h"

class SNFSMSPTeabagState : public SNFSMState
{
public:
	SNFSMSPTeabagState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float tauntDuration = 6 * .25;
	float pufferDelay = 3 * .25;
	bool hasPufferSoundPlayed = false;
};
