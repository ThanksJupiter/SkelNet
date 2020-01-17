#pragma once

struct SDL_Texture;
struct SDL_Rect;

class SNSprite
{
public:
	SNSprite();
	SNSprite(SDL_Texture* inTex, SDL_Rect* inRect) : texture(inTex), spritesheetSourceRect(inRect) {}

	SDL_Texture* texture;
	SDL_Rect* spritesheetSourceRect;
};
