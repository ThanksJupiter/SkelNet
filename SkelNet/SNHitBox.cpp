#include "SNHitBox.h"
#include "SNEngine.h"
#include <string>
#include "SNCamera.h"

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

bool SNHitBox::CheckCollision(SNHitBox& otherHitBox)
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

void SNHitBox::DrawDebug(SNCamera* cam)
{
	engSetColor(0, 0, 255);
	if (lastState.isTriggered)
	{
		engSetColor(255, 0, 0);
	}

	std::string heyo = std::to_string(id);

	Vector2 newPos = cam->MakePositionWithCam(position);

	engDrawString({ newPos.x, newPos.y - 20 }, heyo.c_str());

	engDrawLine(newPos, { newPos.x + size.x, newPos.y });
	engDrawLine(newPos, { newPos.x, newPos.y + size.y });
	engDrawLine(newPos + size, { newPos.x + size.x, newPos.y });
	engDrawLine(newPos + size, { newPos.x, newPos.y + size.y });
	engSetColor(0, 0, 0);
}


