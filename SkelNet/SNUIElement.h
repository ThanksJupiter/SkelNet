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
	void UpdateText(std::string text);
	void UpdateText(float value);
	void UpdateText(int value);
	void UpdateText(bool value);

	void DrawDebug();

	std::function<void()> OnClicked;

	std::string textString;
	float textScaleMultiplier = 1.0f;
	bool isClickable;
	bool drawRect;

	bool hidden = false;

	SNAnchor anchor;
	Vector2 size;
	Vector2 position;
	Vector2 anchorOffset;
	bool isUsed = false;
};