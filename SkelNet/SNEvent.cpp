#include "SNEvent.h"

SNEvent::SNEvent(void(*function)(void))
	: delegatedFunction(function)
{}

void SNEvent::Invoke()
{
	delegatedFunction();
}