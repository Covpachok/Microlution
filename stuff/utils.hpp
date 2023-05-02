//
// Created by heylc on 12.01.2023.
//

#ifndef MICROLUTION_UTILS_HPP
#define MICROLUTION_UTILS_HPP

#include "raylib.h"
#include <iostream>
#include <vector>

template<typename T>
using Array2D = std::vector<std::vector<T>>;

inline float GetRandomFloat(float min, float max)
{
	return static_cast<float>(GetRandomValue(static_cast<int>(min * 100), static_cast<int>(max * 100))) / 100.f;
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
