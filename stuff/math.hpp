#ifndef MICROLUTION_MATH_HPP
#define MICROLUTION_MATH_HPP

#include "raymath.h"
#include <cmath>

inline float V2AngleBetween(const Vector2 &l, const Vector2 &r)
{
	float dot   = l.x * r.x + l.y * r.y;
	float cross = l.x * r.y - l.y * r.x;
	return std::atan2(cross, dot);
}

inline bool V2Equals(const Vector2 &lhs, const Vector2 &rhs, float precision = 0.01f)
{
	return abs(lhs.x - rhs.x) <= precision && abs(lhs.y - rhs.y) <= precision;
}

inline bool FEquals(float lhs, float rhs, float precision = 0.01f)
{
	return abs(lhs - rhs) <= precision;
}

#endif //MICROLUTION_MATH_HPP
