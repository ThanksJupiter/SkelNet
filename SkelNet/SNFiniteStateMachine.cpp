#include "SNFiniteStateMachine.h"
#include "SNFSMState.h"
#include "SNFSMData.h"


SNFiniteStateMachine::SNFiniteStateMachine(SNFSMData* inFsmData)
{
	fsmData = inFsmData;
}

void SNFiniteStateMachine::Update(float dt)
{
	currentState->Update(dt, fsmData);
}

void SNFiniteStateMachine::EnterState(Uint8 newState)
{
	if (currentState)
	{
		currentState->Exit(fsmData);
	}
	// TODO: Save NewState as Uint8 for comparisons instead of comparing states
	currentState = fsmData->availableStates[newState];
	currentState->Enter(fsmData);
}
