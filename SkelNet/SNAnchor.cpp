#include "SNAnchor.h"
#include "SNEngine.h"
#include <stdio.h>

void SNAnchor::SetParent(SNAnchor& anchor)
{
	parent = &anchor;
	parent->AddChild(*this);
}

void SNAnchor::AddChild(SNAnchor& child)
{
	if (CONNECTED_CHILDREN >= MAX_CONNECTED_CHILDREN)
		return;

	for (int i = 0; i < MAX_CONNECTED_CHILDREN; ++i)
	{
		if (children[i] == nullptr)
		{
			children[i] = &child;
			children[i]->SetParentOffset(children[i]->parent->GetAbsolutePosition() - absolutePosition); // TODO: Offset is 0
			children[i]->isUsed = true;
			return;
		}
	}
}

void SNAnchor::UpdatePosition()
{
	if (parent != nullptr)
	{
		absolutePosition = parent->GetAbsolutePosition() + parentOffset;
	}

	for (int i = 0; i < MAX_CONNECTED_CHILDREN; ++i)
	{
		if (children[i] != nullptr && children[i]->isUsed)
		{
			children[i]->UpdatePosition();
		}
	}
}

void SNAnchor::SetParentOffset(Vector2 offset)
{
	parentOffset = offset;
}

void SNAnchor::SetAbsolutePosition(Vector2 position)
{
	absolutePosition = position;
	if (parent != nullptr)
	{
		SetParentOffset(parent->GetAbsolutePosition() - absolutePosition);
	}
}

void SNAnchor::SetRelativePosition(Vector2 position)
{
	absolutePosition = parent->GetAbsolutePosition() + position;
}

Vector2 SNAnchor::GetAbsolutePosition()
{
	return absolutePosition;
}

void SNAnchor::DrawDebug(bool drawDebugInChildren)
{

	if (parent != nullptr)
	{
		engSetColor(255, 0, 0);
		engDrawPoint(parent->absolutePosition, 5.f);
		engSetColor(0, 255, 0);
		engDrawPoint(absolutePosition, 5.f);
		engSetColor(255, 0, 0);
		engDrawArrow(parent->absolutePosition, absolutePosition);
		engSetColor(0, 0, 0);

	}

	if (!drawDebugInChildren)
		return;

	for (int i = 0; i < MAX_CONNECTED_CHILDREN; ++i)
	{
		if (children[i] != nullptr && children[i]->isUsed)
		{
			children[i]->DrawDebug(true);
		}
	}
}
