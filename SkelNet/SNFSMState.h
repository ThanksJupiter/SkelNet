#pragma once

struct SNFSMData;

class SNFSMState
{
public:
	SNFSMState(const char* inName) : stateName(inName) {}

	virtual void Enter(SNFSMData* fsmData) = 0;
	virtual void Update(float dt, SNFSMData* fsmData) = 0;
	virtual void Exit(SNFSMData* fsmData) = 0;

	const char* stateName;
};
