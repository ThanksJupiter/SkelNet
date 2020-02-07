#pragma once
#include "SNFSMState.h"
class SNFSMAPDootState : public SNFSMState
{
public:
	SNFSMAPDootState(const char* inName) : SNFSMState(inName) {}

	void Enter(SNFSMData* fsmData);
	void Update(float dt, SNFSMData* fsmData);
	void Exit(SNFSMData* fsmData);

	float timer = 0.0f;
	float dootDuration = 1.5f;
};
