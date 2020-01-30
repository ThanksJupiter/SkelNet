#pragma once

#include "SNFSMIdleState.h"
#include "SNFSMWalkState.h"
#include "SNFSMRunState.h"
#include "SNFSMAttackState.h"
#include "SNFSMJumpState.h"
#include "SNFSMKnockbackState.h"
#include "SNFSMFallState.h"

#define MAX_FSM_STATES 10

#define IDLE_STATE 0
#define WALK_STATE 1
#define RUN_STATE 2
#define ATTACK_STATE 3
#define JUMP_STATE 4
#define KNOCKBACK_STATE 5
#define FALL_STATE 6
#define KNOCKDOWN_STATE 7
#define TURNAROUND_STATE 8
#define TAUNT_STATE 9

class SNFSMState;
struct SNFSMData;

class SNFiniteStateMachine
{
public:
	SNFiniteStateMachine(SNFSMData* inFsmData);

	SNFSMState* currentState;
	SNFSMData* fsmData;

	SNFSMState* availableStates[MAX_FSM_STATES];

	void Update(float dt);
	void EnterState(Uint8 newState);
};
