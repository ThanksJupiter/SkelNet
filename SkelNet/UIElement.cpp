#include "UIElement.h"
#include "Engine.h"

void UIElement::Draw()
{
	engSetColor(255, 0, 200);
	engDrawRect(size.x, size.y, position.x, position.y);
	engSetColor(0, 0, 0);
}

void UIElement::CheckInteractedWith()
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

void UIElement::SetAbsolutePosition(Vector2 position)
{
	this->position = position;
	anchorOffset = anchor.GetAbsolutePosition() - position;
}

void UIElement::SetRelativePosition(Vector2 position)
{
	this->position = anchor.GetAbsolutePosition() + position;
	anchorOffset = this->position - anchor.GetAbsolutePosition();
}

void UIElement::SetAnchorPosition(Vector2 position)
{
	anchor.SetRelativePosition(position);
}

void UIElement::UpdatePosition()
{
	this->position = anchor.GetAbsolutePosition() + anchorOffset;
}

void UIElement::DrawDebug()
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

	//engSetColor(0, 0, 255);
	//engDrawArrow(anchor.GetAbsolutePosition(), position);
	engSetColor(0, 0, 0);
}
