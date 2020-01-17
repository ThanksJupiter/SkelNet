#include "Engine.h"
#include "Keys.h"
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
#include "SDL_mixer.h"

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Surface* image;
SDL_Texture* texture;

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 500;

SDL_Event event;
const Uint8* state = SDL_GetKeyboardState(NULL);
bool quit = false;

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

	//SOUND
	{
		if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
		{
			fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError()); exit(1);
		}

		Mix_Music* Music;
		if (!(Music = Mix_LoadMUS("Song.wav")))
		{
			fprintf(stderr, "Unable to Find audio source: %s\n", Mix_GetError()); exit(1);
		}
	
		Mix_PlayMusic(Music, -1);
	}
	if (!IMG_Init(IMG_INIT_PNG))
	{
		std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
	}

	window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (!renderer)
		std::cout << SDL_Error << std::endl;

	image = IMG_Load("spritesheet.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);

	TTF_Init();

	StandardFont = TTF_OpenFont("bin/FrizQuadrataTT.ttf", 24);

	if (!StandardFont) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
	}
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
	// access animators from here?

	//animators.DisplayAnimation();

	/*Uint32 ticks = SDL_GetTicks();
	Uint32 seconds = ticks / 200;
	Uint32 sprite = seconds % 2; // + offset for first frame & modulus for frames num

	SDL_Rect srcrect = { sprite * 32, 0, 32, 32 };
	SDL_Rect dstrect = { 0, 0, 128, 128 };

	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);*/
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}

void engUpdate()
{
	engDrawString(100, 100, "remoulad");
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
	CurrentColor.r = Red;
	CurrentColor.g = Green;
	CurrentColor.b = Blue;
	CurrentColor.a = 255;
}

void engDrawString(int X, int Y, const char* String)
{
	// Render text onto surface
	SDL_Surface* MsgSurface = TTF_RenderText_Solid(StandardFont, String, CurrentColor);
	SDL_Texture* MsgTexture = SDL_CreateTextureFromSurface(renderer, MsgSurface);

	// Find out dimensions
	int MsgW = 0, MsgH = 0;
	TTF_SizeText(StandardFont, String, &MsgW, &MsgH);

	// Copy that bad boy
	SDL_Rect MessageRect = { X, Y, MsgW, MsgH };
	SDL_RenderCopy(renderer, MsgTexture, NULL, &MessageRect);

	// Remember to clean up
	SDL_FreeSurface(MsgSurface);
	SDL_DestroyTexture(MsgTexture);
}