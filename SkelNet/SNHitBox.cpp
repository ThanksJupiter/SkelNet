#include "SNHitBox.h"
#include "SNEngine.h"

struct TriggerState
{
	bool isTriggered;
	int frameNum;
};

TriggerState lastState;

void SNHitBox::Setup(Vector2 position, Vector2 size, Vector2 offset, bool blocking, bool callDelegates, std::function<void()> OnTriggerEnter, std::function<void()> OnTriggerExit)
{
	this->position = position + offset;
	this->size = size;
	this->offset = offset;
	this->blocking = blocking;
	this->callDelegates = callDelegates;
	this->OnTriggerEnter = OnTriggerEnter;
	this->OnTriggerExit = OnTriggerExit;

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
		if (lastState.isTriggered == false)
		{
			printf("OnTriggerEnter\n");
			if (callDelegates)
			{
				OnTriggerEnter();
			}
			lastState.isTriggered = true;
			isColliding = true;
		}

		return true;
	}
	else
	{
		if (lastState.isTriggered == true)
		{
			printf("OnTriggerExit\n");
			if (callDelegates)
			{
				OnTriggerExit();
			}
			lastState.isTriggered = false;
			isColliding = false;
		}

		return false;
	}
}

void SNHitBox::CheckCollisions(const SNHitBox* hitBoxArray, const int numHitBoxes)
{

}

void SNHitBox::SetPosition(Vector2 position)
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
	if (isColliding)
	{
		engSetColor(255, 0, 0);
	}

	engDrawLine(position, { position.x + size.x, position.y });
	engDrawLine(position, { position.x, position.y + size.y });
	engDrawLine(position + size, { position.x + size.x, position.y });
	engDrawLine(position + size, { position.x, position.y + size.y });
	engSetColor(0, 0, 0);
}


