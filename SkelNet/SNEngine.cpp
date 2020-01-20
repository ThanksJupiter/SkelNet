#include "SNEngine.h"
#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <SDL_net.h>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <unordered_map>
#include <array>
#include "Key.h"
#include "SDL_ttf.h"
#include "SNSprite.h"
#include "SNAudioManager.h"

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Surface* image;
SDL_Texture* texture;

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 500;

SDL_Event event;
const Uint8* state = SDL_GetKeyboardState(NULL);
bool quit = false;

//Sounds
SNAudioManager* audioManager;

struct InputState
{
	bool pressed;
	int frameNum;
};
static InputState keyStates[(unsigned int)Key::MAX];

static InputState mouseStates[3];

void engInit()
{
	SDL_Init(SDL_INIT_VIDEO);

	if (!IMG_Init(IMG_INIT_PNG))
	{
		std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
	}

	window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (!renderer)
		std::cout << SDL_Error << std::endl;

	image = IMG_Load("SN_Skel_Attack-Sheet.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);

	//intialize Text (frames)
	TTF_Init();

	//init font
	if (!(standardFont = TTF_OpenFont("bin/FrizQuadrataTT.ttf", 24))) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}

	//Init audio manager
	audioManager = new SNAudioManager;
	audioManager->InitSounds();
}

SDL_Texture* engLoadTexture(const char* path)
{
	SDL_Surface* image = IMG_Load(path);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	return texture;
}

void engClose()
{
	SDL_FreeSurface(image);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void engRender()
{
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}

void engUpdate()
{
	currentFrameNum++;

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			quit = true;

		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.repeat == 0)
			{
				InputState& state = keyStates[e.key.keysym.scancode];
				state.pressed = true;
				state.frameNum = currentFrameNum;
			}
		}

		if (e.type == SDL_KEYUP)
		{
			InputState& state = keyStates[e.key.keysym.scancode];
			state.pressed = false;
			state.frameNum = currentFrameNum;
		}

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.clicks == 1)
			{
				InputState& state = mouseStates[e.button.button];
				state.pressed = true;
				state.frameNum = currentFrameNum;
			}
		}
		if (e.type == SDL_MOUSEBUTTONUP)
		{
			InputState& state = mouseStates[e.button.button];
			state.pressed = false;
			state.frameNum = currentFrameNum;
		}
	}
}

bool engShouldQuit()
{
	return quit;
}

int engGetHeight()
{
	return WINDOW_HEIGHT;
}

int engGetWidth()
{
	return WINDOW_WIDTH;
}

int engGetFrameNum()
{
	return currentFrameNum;
}

void engSetColor(unsigned char red, unsigned char green, unsigned char blue)
{
	SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
}

void engDrawRect(int width, int height, int x, int y)
{
	SDL_Rect rect;
	rect.h = height;
	rect.w = width;
	rect.x = x;
	rect.y = y;

	SDL_RenderFillRect(renderer, &rect);
}

void engDrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void engDrawLine(Vector2 pos1, Vector2 pos2)
{
	SDL_RenderDrawLine(renderer, pos1.x, pos1.y, pos2.x, pos2.y);
}

void engDrawPoint(Vector2 position, float radius)
{
	{
		Vector2 pointToRotate;
		pointToRotate.x = radius;
		pointToRotate.y = 0.f;

		Vector2 prevPoint = pointToRotate;

		float delta = (2.f * PI) / 10;

		for (int i = 0; i < 11; ++i)
		{
			float s = sin(delta * i);
			float c = cos(delta * i);

			Vector2 newPos;
			newPos.x = pointToRotate.x * c - pointToRotate.y * s;
			newPos.y = pointToRotate.x * s + pointToRotate.y * c;

			engDrawLine(position + prevPoint, position + newPos);
			prevPoint = newPos;
		}
	}

	{
		radius /= 2.f;
		engDrawLine({ position.x + radius, position.y + radius }, { position.x - radius, position.y - radius });
		engDrawLine({ position.x + radius, position.y - radius }, { position.x - radius, position.y + radius });
	}
}

void engDrawSprite(SDL_Rect& srcRect, SDL_Rect& dstRect, bool flip)
{
	SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void engDrawSprite(SNSprite& image, Vector2 drawPosition, Vector2 drawScale, bool flip)
{
	SDL_Rect destinationRect = { drawPosition.x - drawScale.x, drawPosition.y - drawScale.y, drawScale.x, drawScale.y };

	SDL_RenderCopyEx(renderer, image.texture, &image.sheetSourceRect, &destinationRect, 0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void engDrawSprite(SDL_Rect& srcRect, SDL_Rect& dstRect, SDL_Texture* inTex, bool flip)
{
	SDL_RenderCopyEx(renderer, inTex, &srcRect, &dstRect, 0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void engDrawArrow(Vector2 startPosition, Vector2 endPosition)
{
	engDrawLine(startPosition, endPosition);

	Vector2 difVector = endPosition - startPosition;
	Vector2 crossVector = Cross(difVector);

	float wingSpread = Length(difVector * 0.04f);
	float wingLength = Length(difVector * 0.9f);

	engDrawLine(endPosition, startPosition + Normalize(difVector) * wingLength + Normalize(crossVector) * wingSpread);
	engDrawLine(endPosition, startPosition + Normalize(difVector) * wingLength - Normalize(crossVector) * wingSpread);
}

void engDrawArrow(Vector2 startPosition, Vector2 direction, float length)
{
	engDrawArrow(startPosition, direction * length);
}

bool engGetKey(Key inKey)
{
	return keyStates[(int)inKey].pressed;
}

bool engGetKeyDown(Key inKey)
{
	InputState& state = keyStates[(int)inKey];
	return state.pressed && state.frameNum == currentFrameNum;
}

Vector2 engGetMousePosition()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return { (float)x, (float)y };
}

bool engGetMouseButton(MouseButton inButton)
{
	return mouseStates[(int)inButton].pressed;
}

bool engGetMouseButtonDown(MouseButton inButton)
{
	InputState& state = mouseStates[(int)inButton];
	return state.pressed && state.frameNum == currentFrameNum;
}

void engSetTextColor(Uint8 Red, Uint8 Green, Uint8 Blue)
{
	currentColor.r = Red;
	currentColor.g = Green;
	currentColor.b = Blue;
	currentColor.a = 255;
}

void engDrawString(Vector2 position, const char* string)
{
	// Render text onto surface
	SDL_Surface* msgSurface = TTF_RenderText_Solid(standardFont, string, currentColor);
	SDL_Texture* msgTexture = SDL_CreateTextureFromSurface(renderer, msgSurface);

	// Find out dimensions
	int msgW = 0, msgH = 0;
	TTF_SizeText(standardFont, string, &msgW, &msgH);

	// Copy that bad boy
	SDL_Rect messageRect = { position.x, position.y, msgW, msgH };
	SDL_RenderCopy(renderer, msgTexture, NULL, &messageRect);

	// Remember to clean up
	SDL_FreeSurface(msgSurface);
	SDL_DestroyTexture(msgTexture);
}

Vector2 engGetTextSize(const char* string)
{
	int msgW = 0, msgH = 0;
	TTF_SizeText(standardFont, string, &msgW, &msgH);

	return { (float)msgW, (float)msgH };
}
