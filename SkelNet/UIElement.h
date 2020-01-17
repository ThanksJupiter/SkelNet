#pragma once
#include "Vector.h"
#include "Anchor.h"
#include <functional>

class UIElement
{
public:
	UIElement() {};

	UIElement(Vector2 position, Vector2 size);
	UIElement(Vector2 position, Vector2 size, bool isClickable, std::function<void()> OnClicked);
	void Draw();
	void CheckInteractedWith();

	void SetAbsolutePosition(Vector2 position);
	void SetRelativePosition(Vector2 position);
	void SetAnchorPosition(Vector2 position);
	void UpdatePosition();

	void DrawDebug();

	std::function<void()> OnClicked;

	Anchor anchor;

	bool isClickable;
	Vector2 size;
	Vector2 position;
	Vector2 anchorOffset;
	bool isUsed = false;
};