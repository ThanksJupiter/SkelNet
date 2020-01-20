#include "SpritesheetData.h"

SpritesheetData::SpritesheetData(const char* inPath, int inFrameNo, int inWidth, int inHeight)
{
	filePath = inPath;
	numberOfFrames = inFrameNo;
	cellWidth = inWidth;
	cellHeight = inHeight;
}
