#include "Ship.h"
#include "SNEngine.h"
#include "Vector.h"
#include "SNWorld.h"
#include "Bullet.h"
#include <math.h>

void Ship::Spawn(Vector2 initPos, SNWorld& world)
{
	position = initPos;
	velocity = {0, 0};
	forward = {1, 0};
	this->world = &world;
}

void Ship::Accelerate()
{
	velocity += (forward * accelerationCoef);
}

void Ship::Deaccelerate()
{
	Vector2 temp = -velocity * breakCoef;
	velocity += temp;
}

void Ship::UpdatePosition()
{
	position += velocity;

	// Loop position on X axis
	if (position.x > engGetWidth())
		position.x = 0;

	if (position.x < 0)
		position.x = engGetWidth();
	
	// Loop position on Y axis
	if (position.y > engGetHeight())
		position.y = 0;

	if (position.y < 0)
		position.y = engGetHeight();
}

void Ship::RotateCCW()
{
	float s = sin(-rotateSpeed);
	float c = cos(-rotateSpeed);

	float xnew = forward.x * c - forward.y * s;
	float ynew = forward.x * s + forward.y * c;

	forward.x = xnew;
	forward.y = ynew;
}

void Ship::RotateCW()
{
	float s = sin(rotateSpeed);
	float c = cos(rotateSpeed);

	float newX = forward.x * c - forward.y * s;
	float newY = forward.x * s + forward.y * c;

	forward.x = newX;
	forward.y = newY;
}

void Ship::CheckInput()
{
	/*if (engGetKey(KeyPress::Left))
		RotateCCW();
	else if (engGetKey(KeyPress::Right))
		RotateCW();
	
	if (engGetKey(KeyPress::Up))
		Accelerate();
	else
		Deaccelerate();

	if (engGetKey(KeyPress::Space))
		Fire(); // TODO: Is called twice (probably on down and up)*/
}

void Ship::Fire()
{
	//world->SpawnBullet(position, forward);
}

void Ship::Draw()
{
	// Draw Ship
	engSetColor(255, 0, 0);

	float rightS = sin(2.5);
	float rightC = cos(2.5);

	float rightX = forward.x * rightC - forward.y * rightS;
	float rightY = forward.x * rightS + forward.y * rightC;

	float leftS = sin(-2.5);
	float leftC = cos(-2.5);
		  
	float leftX = forward.x * leftC - forward.y * leftS;
	float leftY = forward.x * leftS + forward.y * leftC;
	
	Vector2 right = {rightX, rightY};
	Vector2 left = {leftX, leftY};

	engDrawLine(position.x + right.x * 25, position.y + right.y * 25, position.x + forward.x * 30, position.y + forward.y * 30);
	engDrawLine(position.x + left.x * 25, position.y + left.y * 25, position.x + forward.x * 30, position.y + forward.y * 30);

	engDrawLine(position.x + right.x * 25, position.y + right.y * 25, position.x, position.y);
	engDrawLine(position.x + left.x * 25, position.y + left.y * 25, position.x, position.y);
	
	engDrawLine(position.x, position.y, position.x + forward.x * 30, position.y + forward.y * 30);
	
	// Draw Velocity
	//engSetColor(0, 255, 0);
	//engDrawLine(position.x, position.y, position.x + forward.x * 50, position.y + forward.y * 50);

	

	engSetColor(0, 0, 0);
}

void Ship::Update()
{
	CheckInput();
	UpdatePosition();
}
