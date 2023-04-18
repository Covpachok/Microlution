//
// Created by heylc on 18.04.2023.
//

#ifndef MICROLUTION_CONSTANTS_HPP
#define MICROLUTION_CONSTANTS_HPP

#include <cstdint>

namespace Constants
{
	constexpr size_t ScreenWidth      = 1280;
	constexpr size_t ScreenHeight     = 720;
	constexpr size_t HalfScreenWidth  = ScreenWidth / 2;
	constexpr size_t HalfScreenHeight = ScreenHeight / 2;
	constexpr size_t CellSize         = 40;
	constexpr size_t GridWidth        = ScreenWidth / CellSize;
	constexpr size_t GridHeight       = ScreenHeight / CellSize;
}

#endif //MICROLUTION_CONSTANTS_HPP
