#include "SNTrailRenderer.h"
#include "SNEngine.h"

struct Segment
{
	Vector2 position;
	Vector2 size;
	float rotation;
};

void SNTrail::Setup(Vector2 initPos, int numSegments, float length, float startWidth, float endWidth, SNSprite& sprite)
{
	position = initPos;
	this->numSegments = numSegments;
	this->length = length;
	this->startWidth = startWidth;
	this->endWidth = endWidth;
	this->sprite = &sprite;

	//segments = std::vector<Segment>(numSegments);
}

void SNTrail::Draw()
{
	for (int i = 1; i < numSegments; ++i)
	{
		/*SDL_Rect dstRect;
		dstRect.x = segments[i].position.x;
		dstRect.x = segments[i].position.x;
		dstRect.w = segments[i].size.x;
		dstRect.h = segments[i].size.y;
*/

		//engDrawSprite(sprite->sheetSourceRect, dstRect, sprite->texture, false, segments[i].rotation, { 0,0 });


		//Vector2 pos1 = segments[i - 1].position;
		//Vector2 pos2 = segments[i].position;

		//engDrawArrow(pos1, pos2);
	}
}

void SNTrail::Update(Vector2 anchorPosition)
{
	
}
