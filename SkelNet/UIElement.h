#pragma once
#include "Vector.h"
#include "Anchor.h"
#include <functional>

class UIElement
{
public:
	UIElement() {};

	void Draw();
	void CheckInteractedWith();

	void SetAbsolutePosition(Vector2 position);
	void SetRelativePosition(Vector2 position);
	void SetAnchorPosition(Vector2 position);
	void UpdatePosition();

	void DrawDebug();

	std::function<void()> OnClicked;

	const char* textString;
	bool isClickable;
	bool drawRect;

	Anchor anchor;
	Vector2 size;
	Vector2 position;
	Vector2 anchorOffset;
	bool isUsed = false;
};