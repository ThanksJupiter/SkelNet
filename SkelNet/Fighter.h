 #pragma once

#include "Vector.h"

class World;
class Animator;

class Fighter
{
public:
	void Spawn(Vector2 initPos, World& world);

	void UpdatePosition();
	void SetPosition(Vector2 newPosition);
	void CheckInput();
	void Draw();
	void Update();

	bool flip;
	Vector2 previousPosition;
	Vector2 position;
	Vector2 velocity;

	World* world;
	
	Animator* animator;

};
