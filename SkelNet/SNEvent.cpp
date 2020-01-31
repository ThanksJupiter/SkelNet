#include "SNEvent.h"
#include "SNWorld.h"

void SNEvent::Setup(SNWorld* world, void (SNWorld::*function)(void))
{
	this->world = world;
	delegatedFunction = function;
}

void SNEvent::Invoke()
{
	(world->*delegatedFunction)();
}