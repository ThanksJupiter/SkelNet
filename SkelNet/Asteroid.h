#pragma once
#include "Vector.h"

class Asteroid
{
public:
	void Spawn();
	void SetRandomPosition();
	void SetRandomVelocity();
	void SetRandomShape();
	void Update();
	void Draw();

	bool CheckIfCollide(Vector2 otherPos);

	Vector2 position;
	Vector2 velocity;
	float speed = 0.01f;
	
	static const int shapeResloution = 10;
	int maxSize = 50;
	Vector2 drawPoints[shapeResloution];

	short tag;
	int index;
	bool destroyed = true;
	bool isColliding = false;
};