#pragma once

#include "SNFSMAPIdleState.h"
#include "SNFSMAPWalkState.h"
#include "SNFSMAPRunState.h"
#include "SNFSMAPAttackState.h"
#include "SNFSMAPJumpStartState.h"
#include "SNFSMAPKnockbackState.h"
#include "SNFSMAPFallState.h"

#define MAX_FSM_STATES 17

#define IDLE_STATE 0
#define WALK_STATE 1
#define RUN_STATE 2
#define ATTACK_STATE 3
#define JUMP_START_STATE 4
#define KNOCKBACK_STATE 5
#define FALL_STATE 6
#define KNOCKDOWN_STATE 7
#define TURNAROUND_STATE 8
#define TEABAG_STATE 9
#define JUMPSQUAT_STATE 10
#define LAND_STATE 11
#define DEATH_STATE 12
#define JUMP_ASCEND_STATE 13
#define SMOKE_STATE 14
#define DOOT_STATE 15
#define DASH_ATTACK_STATE 16

class SNFSMState;
struct SNFSMData;

class SNFiniteStateMachine
{
public:
	SNFiniteStateMachine(SNFSMData* inFsmData);

	SNFSMState* currentState;
	Uint8 currentStateIndex;
	Uint8 previousStateIndex;
	SNFSMData* fsmData;

	SNFSMState* availableStates[MAX_FSM_STATES];

	void Update(float dt);
	void EnterState(Uint8 newState);
};
