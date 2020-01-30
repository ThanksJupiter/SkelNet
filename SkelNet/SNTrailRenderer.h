#pragma once
#include "Vector.h"
#include "SNSprite.h"

struct Segment;

class SNTrail
{
public:
	void Setup(Vector2 initPos, int numSegments, float length, float startWidth, float endWidth, SNSprite& sprite);
	void Draw(SNCamera* cam);
	void Update(Vector2 anchorPosition);

	Vector2 position;

	int numSegments;
	float length;
	float startWidth;
	float endWidth;

	SNSprite* sprite;
	//std::vector<Segment> segments;
};