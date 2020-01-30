#include "SNFloor.h"
#include "SNEngine.h"
#include "Vector.h"
#include "SNWorld.h"
#include <math.h>

void SNFloor::Spawn(Vector2 pos, Vector2 size, SNWorld* world)
{
	transform.SetPosition(pos);
	transform.SetScale(size);
	this->world = world;
}

void SNFloor::Draw()
{
	SDL_Rect dstRect;
	dstRect.w = world->levelSprite->width;
	dstRect.h = world->levelSprite->height;

	Vector2 newPos = transform.GetPosition();

	newPos = world->mainCamera.MakePositionWithCam(transform.GetPosition());
	dstRect.x = newPos.x;
	dstRect.y = newPos.y;

	engDrawSprite(world->levelSprite->sheetSourceRect, dstRect, world->levelSprite->texture);
}