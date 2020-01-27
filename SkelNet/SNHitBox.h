#pragma once
#include "Vector.h"
#include <functional>

struct TriggerState
{
	bool isTriggered;
	int frameNum;
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
	TriggerState lastState;
	TriggerState currentState;
	char id;

	std::function<void()> OnTriggerEnter;
	std::function<void()> OnTriggerExit;

	void Setup(Vector2 position, Vector2 size, Vector2 offset = {0,0}, char id = -1, bool blocking = false, bool callDelegates = false, std::function<void()> OnTriggerEnter = nullptr, std::function<void()> OnTriggerExit = nullptr);
	bool CheckCollision(SNHitBox otherHitBox);
	void UpdatePosition(Vector2 position);
	void SetOffset(Vector2 offset);

	void DrawDebug();
};