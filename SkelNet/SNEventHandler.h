#pragma once
class SNEventHandler
{
public:
	SNEventHandler();
	~SNEventHandler();

	const static int MAX_EVENTS = 10;

	SNEvent* events[MAX_EVENTS];

	
	

};

