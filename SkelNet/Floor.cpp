#include "Floor.h"
#include "Engine.h"
#include "Vector.h"
#include "World.h"
#include <math.h>


void Floor::Spawn(Vector2 pos, Vector2 size)
{
	position = pos;
	this->size = size;
}

void Floor::Draw()
{
	engSetColor(255, 0, 0);
	engDrawRect(size.x, size.y, position.x, position.y);
	engSetColor(0, 0, 0);
}