#pragma once
#include "Vector.h"
#include "SNAnchor.h"
#include <functional>

class SNUIElement
{
public:
	SNUIElement() {};

	void Draw();
	void CheckInteractedWith();

	void SetAbsolutePosition(Vector2 position);
	void SetRelativePosition(Vector2 position);
	void SetAnchorPosition(Vector2 position);
	void UpdatePosition();
	void UpdateText(const char* text);

	void DrawDebug();

	std::function<void()> OnClicked;

	const char* textString;
	bool isClickable;
	bool drawRect;

	SNAnchor anchor;
	Vector2 size;
	Vector2 position;
	Vector2 anchorOffset;
	bool isUsed = false;
};