#pragma once
#include <math.h>

struct he
{
	int n : 2;
	int y : 4;
	int u : 7;
};

class Vector2
{
public:
	float x;
	float y;

	Vector2() {}
	Vector2(const float x, const float y) : x(x), y(y) {}

	Vector2 operator+(Vector2 other) const
	{
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 operator-(Vector2 other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}
	Vector2 operator/(float scalar) const
	{
		return Vector2(x / scalar, y / scalar);
	}
	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}
	Vector2& operator+=(const Vector2& other)
	{
		*this = *this + other;
		return *this;
	}
	Vector2& operator-=(const Vector2& other)
	{
		*this = *this - other;
		return *this;
	}
};

inline float Length(Vector2 vector)
{
	float lengthsqrd = (vector.x * vector.x + vector.y * vector.y);
	return sqrt(lengthsqrd);
}

inline Vector2 Normalize(Vector2 vector)
{
	return vector / Length(vector);
}

inline float Dot(const Vector2& vector, const Vector2& otherVector)
{
	return (vector.x * otherVector.x + vector.y * otherVector.y);
}

inline Vector2 Cross(const Vector2& vector)
{
	return { vector.y, -vector.x };
}