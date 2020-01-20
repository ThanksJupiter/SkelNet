#pragma once

class SNSprite;
struct SDL_Texture;
class SpritesheetData
{
public:
	SpritesheetData(const char* inPath, int inFrameNo, int inWidth, int inHeight);

	const char* filePath;
	int numberOfFrames;
	int cellWidth, cellHeight;
};
