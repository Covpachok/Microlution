//
// Created by heylc on 12.01.2023.
//

#ifndef MICROLUTION_UTILS_HPP
#define MICROLUTION_UTILS_HPP

#include <iostream>
#include <sstream>
#include "raylib.h"
#include "raymath.h"

namespace Constants
{
	constexpr int32_t ScreenWidth      = 1280;
	constexpr int32_t ScreenHeight     = 720;
	constexpr int32_t HalfScreenWidth  = ScreenWidth / 2;
	constexpr int32_t HalfScreenHeight = ScreenHeight / 2;
	constexpr int32_t CellSize         = 16;
	constexpr size_t CellsX = ScreenWidth / CellSize;
	constexpr size_t CellsY = ScreenHeight / CellSize;
}

inline float V2AngleBetween(const Vector2 &l, const Vector2 &r)
{
//	float dot  = Vector2DotProduct(l, r);
//	float len1 = Vector2Length(l);
//	float len2 = Vector2Length(r);
//	return std::acos(dot / ( len1 * len2 ));

//	return std::atan(r.y + r.x) - std::atan(l.y + l.x);

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

class TextureHandler
{
public:
	TextureHandler() :
			mTexture()
	{
		if ( sInstance != nullptr )
		{
			return;
		}
		sInstance = this;
	}

	~TextureHandler()
	{
		UnloadTexture(mTexture);
	}

	void LoadTexture(const std::string &texturePath)
	{
		Image image = LoadImage(texturePath.c_str());

		mTexture = LoadTextureFromImage(image);

		UnloadImage(image);
	}

	const Texture2D &GetTexture() const
	{
		return mTexture;
	}

	static const TextureHandler &GetInstance()
	{
		return *sInstance;
	}

private:
	Texture2D mTexture;

	static TextureHandler *sInstance;
};

#endif //MICROLUTION_UTILS_HPP
