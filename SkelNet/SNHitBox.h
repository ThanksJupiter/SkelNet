#pragma once
#include "Vector.h"
#include <functional>

class SNHitBox
{
public:
	Vector2 position;
	Vector2 size;
	Vector2 offset;
	bool blocking;
	bool callDelegates;
	bool drawDebug;

	std::function<void()> OnTriggerEnter;
	std::function<void()> OnTriggerExit;

	void Setup(Vector2 position, Vector2 size, Vector2 offset = {0,0}, bool blocking = false, bool callDelegates = false, std::function<void()> OnTriggerEnter = nullptr, std::function<void()> OnTriggerExit = nullptr);
	bool CheckCollision(SNHitBox otherHitBox);
	void CheckCollisions(const SNHitBox* hitBoxArray, const int numHitBoxes);
	void SetPosition(Vector2 position);
	void SetOffset(Vector2 offset);

	void DrawDebug();

private:
	bool isColliding;
};