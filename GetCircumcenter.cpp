#include "GetCircumcenter.h"

Vec3 GetCircumcenter(const Vec3& p1, const Vec3& p2, const Vec3& p3)
{
	double a = (p2 - p3).lengthSq();
	double b = (p3 - p1).lengthSq();
	double c = (p1 - p2).lengthSq();

	return ((a * (b + c - a) * p1 + b * (c + a - b) * p2 + c * (a + b - c) * p3) / (a * (b + c - a) + b * (c + a - b) + c * (a + b - c)));
}

Vec3 GetCircumcenter(const Array<Vec3>& positions)
{
	return GetCircumcenter(positions[0], positions[1], positions[2]);
}
