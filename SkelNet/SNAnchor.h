#pragma once
#include "Vector.h"

class SNAnchor
{
public:
	void SetParent(SNAnchor& anchor);
	void AddChild(SNAnchor& child);

	void SetAbsolutePosition(Vector2 position);
	void SetRelativePosition(Vector2 position);
	void UpdatePosition();
	void SetParentOffset(Vector2 offset);
	Vector2 GetAbsolutePosition();
	void DrawDebug(bool drawDebugInChildren);

	SNAnchor* parent;

	static const int MAX_CONNECTED_CHILDREN = 5;
	static const int CONNECTED_CHILDREN = 0;
	SNAnchor* children[MAX_CONNECTED_CHILDREN];

	Vector2 absolutePosition;
	Vector2 parentOffset;
	bool isUsed = false;

};