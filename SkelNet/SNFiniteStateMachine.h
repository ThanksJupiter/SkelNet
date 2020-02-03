#pragma once

#include "SNFSMAPIdleState.h"
#include "SNFSMAPWalkState.h"
#include "SNFSMAPRunState.h"
#include "SNFSMAPAttackState.h"
#include "SNFSMAPJumpState.h"
#include "SNFSMAPKnockbackState.h"
#include "SNFSMAPFallState.h"

#define MAX_FSM_STATES 12

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
#define JUMPSQUAT_STATE 10
#define LAND_STATE 11

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
