#pragma once

#include "SNFSMState.h"

class SNFSMSPTauntState : public SNFSMState
{
public:
	SNFSMSPTauntState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float tauntDuration = 6 * .25;
	float pufferDelay = 3 * .25;
	bool hasPufferSoundPlayed = false;
};
