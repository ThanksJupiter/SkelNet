#pragma once
#include "SDL.h"
#include "Vector.h"
#include "Key.h"
#include "SDL_ttf.h"

class SNSprite;

const float PI = 3.14f;
static int currentFrameNum = 1;

void engInit();
void engClose();
void engRender();
void engUpdate();
bool engShouldQuit();

int engGetHeight();
int engGetWidth();

int engGetFrameNum();

// Draw
void engSetColor(unsigned char red, unsigned char green, unsigned char blue);
void engDrawRect(int width, int height, int x, int y);
void engDrawLine(int x1, int y1, int x2, int y2);
void engDrawLine(Vector2 pos1, Vector2 pos2);
void engDrawPoint(Vector2 position, float radius);
void engDrawSprite(SDL_Rect& srcRect, SDL_Rect& dstRect, bool flip);
void engDrawSprite(SDL_Rect& srcRect, SDL_Rect& dstRect, SDL_Texture* inTex, bool flip);
void engDrawSprite(SNSprite& image, Vector2 drawPosition, Vector2 drawScale, bool flip = false);
void engDrawArrow(Vector2 startPosition, Vector2 endPosition);
void engDrawArrow(Vector2 startPosition, Vector2 direction, float length);

// Text stuff
static TTF_Font* standardFont = nullptr;
static SDL_Color currentColor = { 255, 255, 255, 255 };

// Input
bool engGetKey(Key inKey);
bool engGetKeyDown(Key inKey);
Vector2 engGetMousePosition();
bool engGetMouseButton(MouseButton inButton);
bool engGetMouseButtonDown(MouseButton inButton);

// Text 
void engSetTextColor(Uint8 red, Uint8 green, Uint8 blue);
void engDrawString(Vector2 position, const char* string);
Vector2 engGetTextSize(const char* string);

// Image loading
SDL_Texture* engLoadTexture(const char* path);
