#pragma once
class SNEvent
{
public:
	SNEvent(void (*function)(void));
	void Invoke();

private:
	void (*delegatedFunction)(void);
};
