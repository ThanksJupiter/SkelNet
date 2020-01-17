#include "Bullet.h"
#include "Engine.h"
#include "Vector.h"
#include <math.h>

void Bullet::Spawn(Vector2 initPosition, Vector2 direction)
{
	position = initPosition;
	velocity = Normalize(direction) * speed;
}

void Bullet::Update()
{
	position += velocity;
}

void Bullet::Draw()
{
	engSetColor(255, 255, 0);
	engDrawRect(5, 5, position.x, position.y);
	engSetColor(0, 0, 0);
}
