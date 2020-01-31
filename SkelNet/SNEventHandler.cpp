#include "SNEventHandler.h"

void SNEventHandler::CreateEvent(void (SNWorld::*function)(void), Uint8 eventFlag)
{
	if (NUM_EVENTS < MAX_EVENTS)
	{
		for (int i = 0; i < MAX_EVENTS; ++i)
		{
			if (!events[i].isUsed)
			{
				events[i].Setup(world, function);
				events[i].flag = eventFlag;
				events[i].isUsed = true;
				NUM_EVENTS++;
				return;
			}
		}
	}
}

void SNEventHandler::RemoveEvent(Uint8 eventFlag)
{

}

void SNEventHandler::InvokeEvent(Uint8 eventFlag)
{
	for (int i = 0; i < MAX_EVENTS; ++i)
	{
		if (events[i].flag == eventFlag)
		{
			events[i].Invoke();
		}
	}
}
