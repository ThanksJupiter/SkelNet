#pragma once
#include "Vector.h"

class World;

class Ship
{
public:
	void Spawn(Vector2 initPos, World& world);
	void Accelerate();
	void Deaccelerate();
	void UpdatePosition();
	void RotateCCW();
	void RotateCW();
	void CheckInput();
	void Fire();
	void Draw();
	void Update();
	
	short tag;
	Vector2 position;
	Vector2 velocity;
	Vector2 forward;
	float maxSpeed = 50.f;
	float accelerationCoef = .0001f;
	float breakCoef = .001f;
	float rotateSpeed = .005f;

	World* world;
};