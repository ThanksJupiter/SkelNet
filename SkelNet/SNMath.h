#pragma once
#include "Vector.h"

struct SNMat3x3
{
	float a, b, c,
		d, e, f,
		g, h, i;
};

inline void LookAt()
{

}

inline Vector2 MultiplyMat3x3Vec2(SNMat3x3 mat, Vector2 vec)
{
	return Vector2{
		mat.a * vec.x + mat.b * vec.y + mat.c * 1,		//x
		mat.d * vec.x + mat.e * vec.y + mat.f * 1,		//y
		//mat.g * vec.x + mat.h * vec.y + mat.i * 1		//z
	};
}

inline Vector2 ScaleVector(Vector2 vec, float scale)
{
	SNMat3x3 mat =
	{
		scale, 0, 0,
		0, scale, 0,
		0, 0, 1
	};

	return MultiplyMat3x3Vec2(mat, vec);
}

inline Vector2 TranslateVector(Vector2 vec, Vector2 translation)
{
	SNMat3x3 mat =
	{
		1, 0, translation.x,
		0, 1, translation.y,
		0, 0, 1
	};

	return MultiplyMat3x3Vec2(mat, vec);
}

inline Vector2 RotateVector(Vector2 vec, float theta)
{
	SNMat3x3 mat =
	{
		cos(theta),	 sin(theta), 0,
		-sin(theta), cos(theta), 0,
		0, 0, 1
	};

	return MultiplyMat3x3Vec2(mat, vec);
}
