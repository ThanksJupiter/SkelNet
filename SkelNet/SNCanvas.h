#pragma once
#include "Vector.h"
#include "SNUIElement.h"
#include <functional>
#include "SNAnchor.h"

class SNCanvas
{
public:
	SNCanvas() {};

	void Setup(Vector2 size, Vector2 initPos, SNAnchor* parentElement = nullptr, Vector2 anchorOffset = { 0.f,0.f });
	void CheckInteraction();
	void SetAnchorPosition(Vector2 position);
	void UpdatePosition();
	void Draw();

	// Create Elements
	SNUIElement* CreateRect(Vector2 position, Vector2 size, SNAnchor* parentElement = nullptr, Vector2 anchorOffset = { 0.f,0.f });
	SNUIElement* CreateButton(Vector2 position, Vector2 size, bool isClickable, std::function<void()> OnClicked, SNAnchor* parentElement = nullptr, Vector2 anchorOffset = { 0.f,0.f });
	SNUIElement* CreateImage(Vector2 position, Vector2 size, SNAnchor* parentElement = nullptr, Vector2 anchorOffset = { 0.f,0.f });
	SNUIElement* CreateText(Vector2 position, const char* text, SNAnchor* parentElement = nullptr, Vector2 anchorOffset = { 0.f,0.f });

	static const int MAX_NUM_UIELEMENTS = 10;
	int NUM_UIELEMENTS = 0;
	SNUIElement uiElements[MAX_NUM_UIELEMENTS];

	SNAnchor anchor;
	Vector2 size;

	bool drawDebug = false;
};