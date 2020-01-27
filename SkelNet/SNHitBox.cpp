#include "SNHitBox.h"
#include "SNEngine.h"
#include <string>

void SNHitBox::Setup(Vector2 position, Vector2 size, Vector2 offset, char id, bool blocking, bool callDelegates, std::function<void()> OnTriggerEnter, std::function<void()> OnTriggerExit)
{
	this->position = position + offset;
	this->size = size;
	this->offset = offset;
	this->blocking = blocking;
	this->callDelegates = callDelegates;
	this->OnTriggerEnter = OnTriggerEnter;
	this->OnTriggerExit = OnTriggerExit;
	this->id = id;

	lastState.isTriggered = false;
	lastState.frameNum = engGetFrameNum();
}

bool SNHitBox::CheckCollision(SNHitBox otherHitBox)
{
	if (position.x + size.x >= otherHitBox.position.x &&
		position.x <= otherHitBox.position.x + otherHitBox.size.x &&
		position.y + size.y >= otherHitBox.position.y &&
		position.y <= otherHitBox.position.y + otherHitBox.size.y)
	{
		if (!lastState.isTriggered)
		{
			if (callDelegates)
			{
				OnTriggerEnter();
			}
		}
		currentState.otherId = otherHitBox.id;
		currentState.isTriggered = true;
		currentState.frameNum = engGetFrameNum();
		return true;
	}
	/*else // TODO: Enable if hell breaks loose (and fix it)
	{
		if (lastState.isTriggered)
		{
			if (callDelegates)
			{
				OnTriggerExit();
			}
			//currentState.isTriggered = false;
		}
		currentState.frameNum = engGetFrameNum();
		return false;
	}*/
}

void SNHitBox::UpdatePosition(Vector2 position)
{
	this->position = position + offset;
}

void SNHitBox::SetOffset(Vector2 offset)
{
	this->offset = offset;
}

void SNHitBox::DrawDebug()
{
	engSetColor(0, 0, 255);
	if (lastState.isTriggered)
	{
		engSetColor(255, 0, 0);
	}

	std::string heyo = std::to_string(id);

	engDrawString({ position.x, position.y - 20 }, heyo.c_str());

	engDrawLine(position, { position.x + size.x, position.y });
	engDrawLine(position, { position.x, position.y + size.y });
	engDrawLine(position + size, { position.x + size.x, position.y });
	engDrawLine(position + size, { position.x, position.y + size.y });
	engSetColor(0, 0, 0);
}


