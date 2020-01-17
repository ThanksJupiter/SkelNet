#include "Anchor.h"
#include "Engine.h"
#include <stdio.h>

void Anchor::SetParent(Anchor& anchor)
{
	parent = &anchor;
	parent->AddChild(*this);
}

void Anchor::AddChild(Anchor& child)
{
	if (CONNECTED_CHILDREN >= MAX_CONNECTED_CHILDREN)
		return;

	for (int i = 0; i < MAX_CONNECTED_CHILDREN; ++i)
	{
		if (children[i] == nullptr)
		{
			children[i] = &child;
			children[i]->SetParentOffset(children[i]->GetAbsolutePosition() - absolutePosition);
			return;
		}
	}
}

void Anchor::UpdatePosition()
{
	if (parent != nullptr)
	{
		absolutePosition = parent->GetAbsolutePosition() + parentOffset;
	}

	for (int i = 0; i < MAX_CONNECTED_CHILDREN; ++i)
	{
		if (children[i]->isUsed)
		{
			children[i]->UpdatePosition();
		}
	}
}

void Anchor::SetParentOffset(Vector2 offset)
{
	parentOffset = offset;
}

void Anchor::SetAbsolutePosition(Vector2 position)
{
	absolutePosition = position;
	if (parent != nullptr)
	{
		SetParentOffset(parent->GetAbsolutePosition() - absolutePosition);
	}
}

void Anchor::SetRelativePosition(Vector2 position)
{
	absolutePosition = parent->GetAbsolutePosition() + position;
}

Vector2 Anchor::GetAbsolutePosition()
{
	return absolutePosition;
}

void Anchor::DrawDebug(bool drawDebugInChildren)
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
