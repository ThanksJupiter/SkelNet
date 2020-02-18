#include "SNUIElement.h"
#include "SNEngine.h"
#include <iomanip>
#include <sstream>
#include "SNSprite.h"
#include "SNWorld.h"
#include <cmath>

void SNUIElement::Draw()
{
	if (hidden)
		return;

	Vector2 newPos = position / engGetRenderScale();
	Vector2 newSize = size / engGetRenderScale();

	if (drawRect)
	{
		engSetColor(60, 60, 60);
		engDrawRect(newSize.x, newSize.y, newPos.x, newPos.y);
		engSetColor(0, 0, 0);
	}

	if (sprite != nullptr)
	{
		SDL_Rect dstRect;
		dstRect.x = newPos.x;
		dstRect.y = newPos.y;
		dstRect.w = newSize.x;
		dstRect.h = newSize.y;

		SDL_SetTextureColorMod(sprite->texture, tintR, tintG, tintB);
		engDrawSprite(sprite->sheetSourceRect, dstRect, sprite->texture);
	}

	if (!textString.empty())
	{
		engDrawString(newPos, textString.c_str(), textScaleMultiplier / engGetRenderScale());
	}
}

void SNUIElement::CheckInteractedWith()
{
	if (!isClickable)
		return;

	if (engGetMouseButtonDown(MouseButton::LeftButton))
	{
		Vector2 mousePos = engGetMousePosition();

		if (mousePos.x <= position.x + size.x && mousePos.x >= position.x
			&& mousePos.y <= position.y + size.y && mousePos.y >= position.y)
		{
			OnClicked();
		}
	}
}

void SNUIElement::SetAbsolutePosition(Vector2 position)
{
	this->position = position;
	anchorOffset = anchor.GetAbsolutePosition() - position;
}

void SNUIElement::SetRelativePosition(Vector2 position)
{
	this->position = anchor.GetAbsolutePosition() + position;
	anchorOffset = this->position - anchor.GetAbsolutePosition();
}

void SNUIElement::SetAnchorPosition(Vector2 position)
{
	anchor.SetRelativePosition(position);
}

void SNUIElement::UpdatePosition()
{
	this->position = anchor.GetAbsolutePosition() + anchorOffset;
}

void SNUIElement::UpdateText(const char* text)
{
	textString = text;
}

void SNUIElement::UpdateText(std::string text)
{
	textString = text;
}

void SNUIElement::UpdateText(float value)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << value;
	textString = stream.str();
}

void SNUIElement::UpdateText(int value)
{
	textString = std::to_string(value);
}

void SNUIElement::UpdateText(bool value)
{
	textString = std::to_string(value);
}

void SNUIElement::DrawDebug()
{
	engSetColor(0, 255, 0);
	engDrawLine(position,
		{ position.x + size.x, position.y });

	engDrawLine(position,
		{ position.x, position.y + size.y });

	engDrawLine(position + size,
		{ position.x + size.x, position.y });

	engDrawLine(position + size,
		{ position.x, position.y + size.y });

	if (Length(anchorOffset) > 1.f)
	{
		engSetColor(0, 0, 255);
		engDrawArrow(anchor.GetAbsolutePosition(), position);
	}

	engSetColor(0, 0, 0);
}
