#pragma once
#include "Vector.h"

class Bullet
{
public:	
	void Spawn(Vector2 position, Vector2 direction);
	void Update();
	void Draw();

	Vector2 position;
	Vector2 velocity;
	float speed = 0.1f;

	short tag;
	int index;
	bool destroyed = true;
};