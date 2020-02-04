#include "SNCanvas.h"
#include "SNEngine.h"

void SNCanvas::Setup(Vector2 size, Vector2 initPos, SNAnchor* parentElement, Vector2 anchorOffset)
{
	anchor.SetAbsolutePosition(initPos);
	this->size = size;

	if (parentElement == nullptr)
	{
		anchor.SetParent(anchor);
	}
	else
	{
		anchor.SetParent(*parentElement);
	}
	SetAnchorPosition(initPos);
}

void SNCanvas::CheckInteraction()
{
	for (int i = 0; i < MAX_NUM_UIELEMENTS; ++i)
	{
		if (uiElements[i].isUsed)
		{
			uiElements[i].CheckInteractedWith();
		}
	}
}

void SNCanvas::SetAnchorPosition(Vector2 position)
{
	anchor.SetAbsolutePosition(position);
}

void SNCanvas::UpdatePosition()
{
	for (int i = 0; i < MAX_NUM_UIELEMENTS; ++i)
	{
		if (uiElements[i].isUsed)
		{
			uiElements[i].anchor.UpdatePosition();
			uiElements[i].UpdatePosition();
		}
	}
}

void SNCanvas::Draw()
{
	for (int i = 0; i < MAX_NUM_UIELEMENTS; ++i)
	{
		if (uiElements[i].isUsed)
		{
			uiElements[i].Draw();
		}
	}

	if (drawDebug)
	{
		Vector2 anchorPos = anchor.GetAbsolutePosition();

		// Draw Anchor
		engSetColor(255, 0, 0);
		engDrawPoint(anchorPos, 15.f);

		// Draw Outline
		engSetColor(0, 255, 0);
		engDrawLine(anchorPos, { anchorPos.x + size.x, anchorPos.y });
		engDrawLine(anchorPos, { anchorPos.x, anchorPos.y + size.y });
		engDrawLine(anchorPos + size, { anchorPos.x + size.x, anchorPos.y });
		engDrawLine(anchorPos + size, { anchorPos.x, anchorPos.y + size.y });

		for (int i = 0; i < MAX_NUM_UIELEMENTS; ++i)
		{
			if (uiElements[i].isUsed)
			{
				uiElements[i].DrawDebug();
				uiElements[i].anchor.DrawDebug(true);
			}
		}
	}

	engSetColor(0, 0, 0);
}

SNUIElement* SNCanvas::CreateRect(Vector2 position, Vector2 size, SNAnchor* parentElement, Vector2 anchorOffset)
{
	if (NUM_UIELEMENTS >= MAX_NUM_UIELEMENTS)
		return nullptr;
	else
	{
		for (int i = 0; i < MAX_NUM_UIELEMENTS; ++i)
		{
			if (uiElements[i].isUsed == false)
			{
				if (parentElement == nullptr)
				{
					uiElements[i].anchor.SetParent(anchor);
				}
				else
				{
					uiElements[i].anchor.SetParent(*parentElement);
				}
				uiElements[i].SetAnchorPosition(uiElements[i].anchor.parent->GetAbsolutePosition() + position);
				uiElements[i].SetRelativePosition(anchorOffset);
				uiElements[i].size = size;
				uiElements[i].isUsed = true;
				NUM_UIELEMENTS++;

				return &uiElements[i];
			}
		}
	}
}

SNUIElement* SNCanvas::CreateButton(Vector2 position, Vector2 size, bool isClickable, std::function<void()> OnClicked, SNAnchor* parentElement, Vector2 anchorOffset)
{
	if (NUM_UIELEMENTS >= MAX_NUM_UIELEMENTS)
		return nullptr;
	else
	{
		for (int i = 0; i < MAX_NUM_UIELEMENTS; ++i)
		{
			if (uiElements[i].isUsed == false)
			{
				if (parentElement == nullptr)
				{
					uiElements[i].anchor.SetParent(anchor);
				}
				else
				{
					uiElements[i].anchor.SetParent(*parentElement);
				}
				uiElements[i].SetAnchorPosition(uiElements[i].anchor.parent->GetAbsolutePosition() + position);
				uiElements[i].SetRelativePosition(anchorOffset);
				uiElements[i].size = size;
				uiElements[i].isClickable = isClickable;
				uiElements[i].OnClicked = OnClicked;
				uiElements[i].isUsed = true;
				NUM_UIELEMENTS++;

				return &uiElements[i];
			}
		}
	}
}

SNUIElement* SNCanvas::CreateImage(Vector2 position, Vector2 size, SNAnchor* parentElement, Vector2 anchorOffset)
{

	return nullptr;
}

SNUIElement* SNCanvas::CreateText(Vector2 position, const char* text, float scaleMultiplier, SNAnchor* parentElement, Vector2 anchorOffset)
{
	if (NUM_UIELEMENTS >= MAX_NUM_UIELEMENTS)
		return nullptr;
	else
	{
		for (int i = 0; i < MAX_NUM_UIELEMENTS; ++i)
		{
			if (uiElements[i].isUsed == false)
			{
				if (parentElement == nullptr)
				{
					uiElements[i].anchor.SetParent(anchor);
				}
				else
				{
					uiElements[i].anchor.SetParent(*parentElement);
				}
				uiElements[i].SetAnchorPosition(position);
				uiElements[i].SetRelativePosition(anchorOffset);
				uiElements[i].textString = text;
				uiElements[i].size = engGetTextSize(text) * scaleMultiplier;
				uiElements[i].isUsed = true;
				uiElements[i].textScaleMultiplier = scaleMultiplier;
				NUM_UIELEMENTS++;

				return &uiElements[i];
			}
		}
	}
}
