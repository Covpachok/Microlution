//
// Created by heylc on 03.04.2023.
//

#ifndef MICROLUTION_INTVEC_HPP
#define MICROLUTION_INTVEC_HPP

#include <cmath>
#include <string>

struct IntVec2
{
	int x, y;

	IntVec2(int x = 0, int y = 0) :
			x(x), y(y) {}

	const IntVec2 &operator+=(const IntVec2 &vec);
	const IntVec2 &operator-=(const IntVec2 &vec);
	IntVec2 operator+(const IntVec2 &vec) const;
	IntVec2 operator-(const IntVec2 &vec) const;
	bool operator==(const IntVec2 &vec) const;

	std::string ToString() const;
};

inline const IntVec2 &IntVec2::operator+=(const IntVec2 &vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

inline const IntVec2 &IntVec2::operator-=(const IntVec2 &vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

inline IntVec2 IntVec2::operator+(const IntVec2 &vec) const
{
	return {x + vec.x, y + vec.y};
}

inline IntVec2 IntVec2::operator-(const IntVec2 &vec) const
{
	return {x - vec.x, y - vec.y};
}

inline bool IntVec2::operator==(const IntVec2 &vec) const
{
	return x == vec.x && y == vec.y;
}

inline std::string IntVec2::ToString() const
{
	return "x:" + std::to_string(x) + " y:" + std::to_string(y);
}

const IntVec2 Zero  = IntVec2(0, 0);
const IntVec2 One   = IntVec2(1, 1);
const IntVec2 Up    = IntVec2(0, 1);
const IntVec2 Down  = IntVec2(0, -1);
const IntVec2 Left  = IntVec2(1, 0);
const IntVec2 Right = IntVec2(-1, 0);


inline IntVec2 operator+(const IntVec2 &lhs, const IntVec2 &rhs)
{
	auto ret(lhs);
	ret += rhs;
	return ret;
}

inline IntVec2 operator-(const IntVec2 &lhs, const IntVec2 &rhs)
{
	auto ret(lhs);
	ret -= rhs;
	return ret;
}

inline std::ostream &operator<<(std::ostream &os, const IntVec2 &vec)
{
	os << vec.ToString();
	return os;
}

#endif //MICROLUTION_INTVEC_HPP
