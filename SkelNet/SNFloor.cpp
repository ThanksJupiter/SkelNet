#include "SNFloor.h"
#include "SNEngine.h"
#include "Vector.h"
#include "SNWorld.h"
#include <math.h>


void SNFloor::Spawn(Vector2 pos, Vector2 size)
{
	position = pos;
	this->size = size;
}

void SNFloor::Draw()
{
	engSetColor(255, 0, 0);
	engDrawRect(size.x, size.y, position.x, position.y);
	engSetColor(0, 0, 0);
}