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
#include "SpritesheetData.h"
#include "SNAnimation.h"
#include "SNWorld.h"
#include "SDL_joystick.h"
#include <string>

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Surface* image;
SDL_Texture* texture;

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

SDL_Event event;
const Uint8* state = SDL_GetKeyboardState(NULL);
bool quit = false;

std::string* currentText = nullptr;

struct InputState
{
	bool pressed;
	int frameNum;
};
static InputState keyStates[(unsigned int)Key::MAX];
static InputState buttonStates[unsigned int(GamepadButton::MAX)];

static InputState mouseStates[3];

const float STICK_AXIS_DEADZONE = .18f;
const float SHOULDER_AXIS_DEADZONE = 0.005f;
struct AxisState
{
	float value;
};
static AxisState axisStates[(unsigned int)GamepadAxis::MAX];
static InputState dpadStates[(unsigned int)DPadButton::MAX];

void engInit()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

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

	engGetJoystick();
	axisStates[(unsigned int)GamepadAxis::LeftShoulder].value = -2.0f;
	axisStates[(unsigned int)GamepadAxis::RightShoulder].value = -2.0f;

	//intialize Text (frames)
	TTF_Init();

	//init font
	if (!(standardFont = TTF_OpenFont("joystixmonospace.ttf", 24))) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
}

SDL_Texture* engLoadTexture(const char* path)
{
	SDL_Surface* image = IMG_Load(path);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	return texture;
}

void engLoadAnimationsToWorld(SNWorld& world)
{
	SpritesheetData idleSheet = SpritesheetData("SN_Skel_Idle-Sheet.png", 4, 32, 32);
	SpritesheetData walkSheet = SpritesheetData("SN_Skel_Walk-Sheet.png", 4, 32, 32);
	SpritesheetData runSheet = SpritesheetData("SN_Skel_Run-Sheet.png", 6, 32, 32);
	SpritesheetData attackSheet = SpritesheetData("SN_Skel_Attack-Sheet.png", 12, 100, 30);
	SpritesheetData jumpSheet = SpritesheetData("SN_Skel_Jump_02-Sheet.png", 3, 32, 32);
	SpritesheetData jumpSquatSheet = SpritesheetData("SN_Skel_Jump_Squat-Sheet.png", 2, 32, 32);
	SpritesheetData somersaultSheet = SpritesheetData("SN_Skel_Somersault-Sheet.png", 4, 32, 32);
	SpritesheetData vortexDustSheet = SpritesheetData("SN_Skel_Somersault_Vortex-Sheet.png", 4, 32, 32);
	SpritesheetData knockbackSheet = SpritesheetData("SN_Skel_Knockback.png", 1, 32, 32);
	SpritesheetData dashDustSheet = SpritesheetData("SN_SKel_Dash_Dust-Sheet.png", 8, 32, 32);
	SpritesheetData runDustSheet = SpritesheetData("SN_SKel_Run_Dust-Sheet.png", 8, 32, 32);
	SpritesheetData landingDustSheet = SpritesheetData("SN_Skel_Landing_Dust-Sheet.png", 8, 32, 32);
	SpritesheetData coolDustSheet = SpritesheetData("SN_SKel_Cool_Dust-Sheet.png", 14, 32, 32);
	SpritesheetData fallSheet = SpritesheetData("SN_Skel_Fall-Sheet.png", 4, 32, 32);
	SpritesheetData knockedDownSheet = SpritesheetData("SN_Skel_Knockdown.png", 1, 32, 32);
	SpritesheetData dustCloud01Sheet = SpritesheetData("SN_Skel_Dust_Cloud-Sheet.png", 9, 16, 16);
	SpritesheetData turnAroundsheet = SpritesheetData("SN_Skel_Dash_Stop_02-Sheet.png", 4, 32, 32);
	SpritesheetData teabagSheet = SpritesheetData("SN_Skel_T-Bag-Sheet.png", 6, 32, 32);
	SpritesheetData smokeSheet = SpritesheetData("SN_Skel_Taunt_02-Sheet.png", 20, 64, 64);
	SpritesheetData dootSheet = SpritesheetData("SN_Skel_Doot_01-Sheet.png", 17, 32, 32);
	SpritesheetData deathShockwaveSheet = SpritesheetData("SN_Death_Effect_Shockwave_01-Sheet.png", 7, 128, 128);
	SpritesheetData hitEffect01Sheet = SpritesheetData("SN_Hit_Effect_01-Sheet.png", 2, 32, 32);
	SpritesheetData countDownSheet = SpritesheetData("SN_Game_Start_Countdown-Sheet.png", 4, 64, 32);
	SpritesheetData laughSkelSheet = SpritesheetData("SN_Laugh_Skel_Portrait-Sheet.png", 3, 32, 32);

	SNSprite* idleSprites[4];
	SNSprite* walkSprites[4];
	SNSprite* runSprites[6];
	SNSprite* attackSprites[12];
	SNSprite* jumpSprites[3];
	SNSprite* jumpSquatSprites[2];
	SNSprite* somersaultSprites[4];
	SNSprite* vortexDustSprites[5];
	SNSprite* knockbackSprites[1];
	SNSprite* dashDustSprites[8];
	SNSprite* runDustSprites[8];
	SNSprite* landingDustSprites[14];
	SNSprite* coolDustSprites[14];
	SNSprite* fallSprites[4];
	SNSprite* knockedDownSprites[1];
	SNSprite* dustCloud01Sprites[9];
	SNSprite* turnAroundSprites[4];
	SNSprite* teabagSprites[6];
	SNSprite* smokeSprites[20];
	SNSprite* dootSprites[17];
	SNSprite* deathShockwaveSprites[7];
	SNSprite* hitEffect01Sprites[2];
	SNSprite* countDownSprites[4];
	SNSprite* laughSkelSprites[3];

	world.idleAnim = idleSheet.CreateAnimation(idleSprites, .25);
	world.walkAnim = walkSheet.CreateAnimation(walkSprites, .15);
	world.runAnim = runSheet.CreateAnimation(runSprites, .1);
	world.spAttackAnim = attackSheet.CreateAnimation(attackSprites, .12);
	world.apAttackAnim = attackSheet.CreateAnimation(attackSprites, .12);
	world.jumpAnim = jumpSheet.CreateAnimation(jumpSprites, .15);
	world.jumpSquatAnim = jumpSquatSheet.CreateAnimation(jumpSquatSprites, .06);
	world.somersaultAnim = somersaultSheet.CreateAnimation(somersaultSprites, .08);
	world.vortexDustAnim = vortexDustSheet.CreateAnimation(vortexDustSprites, .1);
	world.knockbackAnim = knockbackSheet.CreateAnimation(knockbackSprites, 1);
	world.dashDustAnim = dashDustSheet.CreateAnimation(dashDustSprites, .05);
	world.runDustAnim = runDustSheet.CreateAnimation(runDustSprites, .05);
	world.landingDustAnim = landingDustSheet.CreateAnimation(landingDustSprites, .05);
	world.coolDustAnim = coolDustSheet.CreateAnimation(coolDustSprites, .05);
	world.fallAnim = fallSheet.CreateAnimation(fallSprites, .15);
	world.knockedDownAnim = knockedDownSheet.CreateAnimation(knockedDownSprites, 1);
	world.dustCloud01Anim = dustCloud01Sheet.CreateAnimation(dustCloud01Sprites, .1);
	world.turnAroundAnim = turnAroundsheet.CreateAnimation(turnAroundSprites, .15);
	world.teabagAnim = teabagSheet.CreateAnimation(teabagSprites, .25);
	world.smokeAnim = smokeSheet.CreateAnimation(smokeSprites, .15);
	world.dootAnim = dootSheet.CreateAnimation(dootSprites, .15);
	world.deathShockwave = deathShockwaveSheet.CreateAnimation(deathShockwaveSprites, .1);
	world.hitEffect01 = hitEffect01Sheet.CreateAnimation(hitEffect01Sprites, .05);
	world.laughSkelAnim = laughSkelSheet.CreateAnimation(laughSkelSprites, .1);

	world.countDownAnim = countDownSheet.CreateAnimation(countDownSprites, 1);

	world.levelSprite = new SNSprite(256, 24, engLoadTexture("SN_Castle_Roof.png"), 0);
	world.ropeSprite = new SNSprite(256, 256, engLoadTexture("SN_Castle_Roof_Double_Rope.png"), 0);
	world.gameTextSprite = new SNSprite(64, 32, engLoadTexture("SN_Game_Finished_Text.png"), 0);
	world.skelNetSprite = new SNSprite(256, 64, engLoadTexture("SN_Splash_Screen_Resize.png"), 0);

	engSubscribeAnimationDelegates(world);
}

void engSubscribeAnimationDelegates(SNWorld& world)
{
	world.runAnim->sprites[0]->shouldNotifyWhenPlayed = true;
	world.runAnim->sprites[0]->animation = world.runDustAnim;

	world.runAnim->sprites[3]->shouldNotifyWhenPlayed = true;
	world.runAnim->sprites[3]->animation = world.runDustAnim;
}

void engSetSpriteRenderScale(float scale)
{
	spriteRenderScale = scale;
}

float engGetSpriteRenderScale()
{
	return spriteRenderScale;
}

void engSetAudioDelegates(SNWorld& world)
{
	// doot
	world.dootAnim->sprites[9]->shouldPlaySound = true;
	world.dootAnim->sprites[9]->audio = world.audioManager->dootSound1;
	world.dootAnim->sprites[9]->audioChannel = world.HasAuthority()? 5 : 6;

	// smoke taunt
	world.smokeAnim->sprites[4]->shouldPlaySound = true;
	world.smokeAnim->sprites[4]->audio = world.audioManager->cigLight;
	world.smokeAnim->sprites[4]->audioChannel = -1;

	world.smokeAnim->sprites[7]->shouldPlaySound = true;
	world.smokeAnim->sprites[7]->audio = world.audioManager->inhale;
	world.smokeAnim->sprites[7]->audioChannel = -1;

	world.smokeAnim->sprites[14]->shouldPlaySound = true;
	world.smokeAnim->sprites[14]->audio = world.audioManager->cigLand;
	world.smokeAnim->sprites[14]->audioChannel = -1;

	// run
	/*world.runAnim->sprites[2]->shouldPlaySound = true;
	world.runAnim->sprites[2]->audio = world.audioManager->land;
	world.runAnim->sprites[2]->audioChannel = -1;

	world.runAnim->sprites[5]->shouldPlaySound = true;
	world.runAnim->sprites[5]->audio = world.audioManager->land;
	world.runAnim->sprites[5]->audioChannel = -1;*/
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
	SDL_RenderSetScale(renderer, renderScale, renderScale);
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
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

		switch (e.type)
		{
			case SDL_JOYAXISMOTION:
			{
				AxisState& state = axisStates[e.jaxis.axis];
				state.value = (float)e.jaxis.value / 32767;
			} break;

			case SDL_JOYBUTTONDOWN:
			{
				InputState& state = buttonStates[e.jbutton.button];
				state.pressed = true;
				state.frameNum = currentFrameNum;
			} break;

			case SDL_JOYBUTTONUP:
			{
				InputState& state = buttonStates[e.jbutton.button];
				state.pressed = false;
				state.frameNum = currentFrameNum;
			} break;	

			case SDL_JOYHATMOTION:
			{
				int hatValue = e.jhat.value;
				if (hatValue != (int)DPadButton::None)
				{
					InputState& state = dpadStates[hatValue];
					state.pressed = true;
					state.frameNum = currentFrameNum;
				}
				else
				{
					for (int i = 0; i < (int)DPadButton::MAX; i++)
					{
						InputState& state = dpadStates[i];
						state.pressed = false;
						state.frameNum = currentFrameNum;
					}
				}
			}
		}

		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.repeat == 0)
			{
				InputState& state = keyStates[e.key.keysym.scancode];
				state.pressed = true;
				state.frameNum = currentFrameNum;
			}

			if (e.key.keysym.sym == SDLK_BACKSPACE && std::string(*currentText).length() > 0)
			{
				currentText->pop_back();
			}
			else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
			{
				SDL_SetClipboardText(currentText->c_str());
			} 
			else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			{
				*currentText = SDL_GetClipboardText();
			}
		}
		else if (e.type == SDL_TEXTINPUT)
		{
			if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
			{
				*currentText += e.text.text;
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

void engSetRenderScale(float newScale)
{
	renderScale = newScale;
}

float engGetRenderScale()
{
	return renderScale;
}

int engGetFrameNum()
{
	return currentFrameNum;
}

void PrintHugeImportantMessage()
{
	std::cout << "xsdddd!! :D (xd)" << std::endl;
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

void engDrawSprite(SDL_Rect& srcRect, SDL_Rect& dstRect, SDL_Texture* inTex, bool flip, float angle, const SDL_Point& rotPoint)
{
	dstRect.w *= spriteRenderScale;
	dstRect.h *= spriteRenderScale;
	SDL_RenderCopyEx(renderer, inTex, &srcRect, &dstRect, angle, &rotPoint, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
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

SDL_Joystick* engGetJoystick()
{
	SDL_JoystickEventState(SDL_ENABLE);

	SDL_Joystick* joystick = SDL_JoystickOpen(0);

	return joystick;
}

bool engGetButton(GamepadButton inButton)
{
	return buttonStates[(int)inButton].pressed;
}

bool engGetButtonDown(GamepadButton inButton)
{
	InputState& state = buttonStates[(int)inButton];
	return state.pressed && state.frameNum == currentFrameNum;
}

float engGetJoystickAxis(GamepadAxis inAxis)
{
	float r = axisStates[(int) inAxis].value;
	return (r > STICK_AXIS_DEADZONE || r < -STICK_AXIS_DEADZONE) ? r : 0;
}

float engGetShoulderAxis(GamepadAxis inAxis)
{
	float r = axisStates[(int)inAxis].value;
	return r > SHOULDER_AXIS_DEADZONE - 1 ? (r / 2) + 0.5f : 0;
}

bool engGetDPadButton(DPadButton inButton)
{
	return dpadStates[(int)inButton].pressed;
}

bool engGetDPadButtonDown(DPadButton inButton)
{
	InputState& state = dpadStates[(int)inButton];
	return state.pressed && state.frameNum == currentFrameNum;
}

void engSetTextColor(Uint8 Red, Uint8 Green, Uint8 Blue)
{
	currentColor.r = Red;
	currentColor.g = Green;
	currentColor.b = Blue;
	currentColor.a = 255;
}

void engDrawString(Vector2 position, const char* string, float scaleMultiplier)
{
	// Render text onto surface
	SDL_Surface* msgSurface = TTF_RenderText_Solid(standardFont, string, currentColor);
	SDL_Texture* msgTexture = SDL_CreateTextureFromSurface(renderer, msgSurface);

	// Find out dimensions
	int msgW = 0, msgH = 0;
	TTF_SizeText(standardFont, string, &msgW, &msgH);

	// Copy that bad boy
	SDL_Rect messageRect = { position.x, position.y, msgW * scaleMultiplier, msgH * scaleMultiplier };
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

std::string engGetInputText()
{
	return *currentText;
}

void engSetInputText(std::string inText)
{
	*currentText = inText;
}

void engSetInputTextPtr(std::string* text)
{
	currentText = text;
}

std::string* engGetInputTextPtr()
{
	return currentText;
}
