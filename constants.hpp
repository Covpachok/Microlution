#ifndef MICROLUTION_CONSTANTS_HPP
#define MICROLUTION_CONSTANTS_HPP

#include <cstdint>

namespace Constants
{
	constexpr size_t ScreenWidth      = 1280;
	constexpr size_t ScreenHeight     = 720;
	// Allowed sizes 5, 8, 10, 16, 20, 40, 80
	constexpr size_t CellSize         = 5;
	constexpr size_t GridWidth        = ScreenWidth / CellSize;
	constexpr size_t GridHeight       = ScreenHeight / CellSize;

	static_assert(ScreenWidth % CellSize == 0 && ScreenHeight % CellSize == 0 && CellSize >= 5, "Width and Height of the screen must be dividable by CellSize");
}

#endif //MICROLUTION_CONSTANTS_HPP
