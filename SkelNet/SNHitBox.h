#pragma once
#include "Vector.h"
#include <functional>
#include <vector>

class SNCamera;

struct SNTriggerState
{
	int frameNum;
	bool isTriggered;
	char otherId;
};

class SNHitBox
{
public:
	Vector2 position;
	Vector2 size;
	Vector2 offset;
	bool blocking;
	bool callDelegates;
	bool drawDebug;
	SNTriggerState lastState;
	SNTriggerState currentState;
	char id;

	std::vector<SNTriggerState> triggerStates;

	std::function<void()> OnTriggerEnter;
	std::function<void()> OnTriggerExit;

	void Setup(Vector2 position, Vector2 size, Vector2 offset = {0,0}, char id = -1, bool blocking = false, bool callDelegates = false, std::function<void()> OnTriggerEnter = nullptr, std::function<void()> OnTriggerExit = nullptr);
	bool CheckCollision(SNHitBox& otherHitBox);
	void UpdatePosition(Vector2 position);
	void SetOffset(Vector2 offset);

	void DrawDebug(SNCamera* cam);
};