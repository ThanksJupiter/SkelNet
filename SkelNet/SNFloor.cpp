#include "SNFloor.h"
#include "SNEngine.h"
#include "Vector.h"
#include "SNWorld.h"
#include <math.h>

void SNFloor::Spawn(Vector2 pos, Vector2 size, SNWorld* world)
{
	pos.x -= (world->levelSprite->width *size.x) / 2;
	transform.SetPosition(pos);
	transform.SetScale(size);
	this->world = world;
}

void SNFloor::Draw()
{
	SDL_Rect dstRect;
	dstRect.w = world->levelSprite->width * transform.GetScale().x;
	dstRect.h = world->levelSprite->height * transform.GetScale().y;

	Vector2 newPos = transform.GetPosition();
	newPos = world->mainCamera.MakePositionWithCam(newPos);
	dstRect.x = newPos.x;
	dstRect.y = newPos.y;

	engDrawSprite(world->levelSprite->sheetSourceRect, dstRect, world->levelSprite->texture);
}