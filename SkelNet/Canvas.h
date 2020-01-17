#pragma once
#include "Vector.h"
#include "UIElement.h"
#include <functional>
#include "Anchor.h"

// TODO: Make Canvas, Handles UI elements like World handles players
class Canvas
{
public:
	Canvas() {};

	void Setup(Vector2 size, Vector2 anchorPos);
	void CheckInteraction();
	void Draw();

	// Create Elements
	UIElement* CreateRect(Vector2 position, Vector2 size, Anchor* parentElement = nullptr);
	UIElement* CreateButton(Vector2 position, Vector2 size, bool isClickable, std::function<void()> OnClicked, Anchor* parentElement = nullptr);
	UIElement* CreateImage(Vector2 position, Vector2 size, UIElement* parentElement = nullptr);
	UIElement* CreateText(Vector2 position, Vector2 size, char* text, UIElement* parentElement = nullptr);

	static const int MAX_NUM_UIELEMENTS = 10;
	int NUM_UIELEMENTS = 0;
	UIElement uiElements[MAX_NUM_UIELEMENTS];

	Anchor anchor;
	Vector2 size;

	bool drawDebug = false;
};