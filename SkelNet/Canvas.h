#pragma once
#include "Vector.h"
#include "UIElement.h"
#include <functional>
#include "Anchor.h"

class Canvas
{
public:
	Canvas() {};

	void Setup(Vector2 size, Vector2 anchorPos);
	void CheckInteraction();
	void Draw();

	// Create Elements
	UIElement* CreateRect(Vector2 position, Vector2 size, Anchor* parentElement = nullptr, Vector2 anchorOffset = {0.f,0.f});
	UIElement* CreateButton(Vector2 position, Vector2 size, bool isClickable, std::function<void()> OnClicked, Anchor* parentElement = nullptr, Vector2 anchorOffset = { 0.f,0.f });
	UIElement* CreateImage(Vector2 position, Vector2 size, Anchor* parentElement = nullptr, Vector2 anchorOffset = { 0.f,0.f });
	UIElement* CreateText(Vector2 position, const char* text, Anchor* parentElement = nullptr, Vector2 anchorOffset = { 0.f,0.f });

	static const int MAX_NUM_UIELEMENTS = 10;
	int NUM_UIELEMENTS = 0;
	UIElement uiElements[MAX_NUM_UIELEMENTS];

	Anchor anchor;
	Vector2 size;

	bool drawDebug = false;
};