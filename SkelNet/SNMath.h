#pragma once

struct SNMat3
{
	int x1, y1, z1;
	int x2, y2, z2;
	int x3, y3, z3;
};

class SNMath
{
	SNMath();
public:
	SNMat3 PostionMatrix;
};
