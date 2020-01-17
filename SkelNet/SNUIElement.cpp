#include "SNUIElement.h"
#include "SNEngine.h"

void SNUIElement::Draw()
{
	if (drawRect)
	{
		engSetColor(255, 0, 200);
		engDrawRect(size.x, size.y, position.x, position.y);
		engSetColor(0, 0, 0);
	}
	

	if (textString != nullptr)
	{
		engDrawString(position, textString);
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
