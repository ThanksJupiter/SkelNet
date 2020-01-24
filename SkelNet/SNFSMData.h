#pragma once

#include "SNFiniteStateMachine.h"
#include "SNFSMState.h"
#include "SNInput.h"
#include "SNFiniteStateMachine.h"
#include "SNAutonomousProxy.h"

class SNWorld;
class SNAutonomousProxy;
class SNSimulatedProxy;
class SNInput;
class SNFSMState;
class SNFiniteStateMachine;

struct SNFSMData
{
	SNFSMData(SNWorld* inWorld, SNAutonomousProxy* inAutoProxy, SNSimulatedProxy* inSimProxy, SNInput* inInput) : world(inWorld), autonomousProxy(inAutoProxy), simulatedProxy(inSimProxy), input(inInput) {}

	SNFiniteStateMachine* stateMachine;
	SNWorld* world;
	SNAutonomousProxy* autonomousProxy;
	SNSimulatedProxy* simulatedProxy;
	SNInput* input;
	SNFSMState* availableStates[10];
};
