#include "SNFiniteStateMachine.h"
#include "SNFSMState.h"


SNFiniteStateMachine::SNFiniteStateMachine(SNFSMData* inFsmData)
{
	fsmData = inFsmData;
}

void SNFiniteStateMachine::Update(float dt)
{
	currentState->Update(dt, fsmData);
}

void SNFiniteStateMachine::EnterState(SNFSMState* newState)
{
	if (currentState)
	{
		currentState->Exit(fsmData);
	}

	currentState = newState;
	currentState->Enter(fsmData);
}
