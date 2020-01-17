#pragma once
#include "Vector.h"

class Anchor
{
public:
	void SetParent(Anchor& anchor);
	void AddChild(Anchor& child);

	void SetAbsolutePosition(Vector2 position);
	void SetRelativePosition(Vector2 position);
	void UpdatePosition();
	void SetParentOffset(Vector2 offset);
	Vector2 GetAbsolutePosition();
	void DrawDebug(bool drawDebugInChildren);

	Anchor* parent;

	static const int MAX_CONNECTED_CHILDREN = 5;
	static const int CONNECTED_CHILDREN = 0;
	Anchor* children[MAX_CONNECTED_CHILDREN];

	Vector2 absolutePosition;
	Vector2 parentOffset;
	bool isUsed = false;

};