#include <vector>
#include "raylib.h"
#include "rlgl.h"
#include "utils.hpp"
#include "logger.hpp"
#include "microbe.hpp"

using namespace std;

void DrawGrid()
{
	Color        color = {0, 0, 0, 16};
	for ( size_t x     = 0; x < Constants::CellsX; ++x )
	{
		size_t startX = x * Constants::CellSize;
		DrawLine(startX, 0, startX, Constants::ScreenHeight, color);
	}

	for ( size_t y = 0; y < Constants::CellsY; ++y )
	{
		size_t startY = y * Constants::CellSize;
		DrawLine(0, startY, Constants::ScreenWidth, startY, color);
	}
}

void DrawOnGrid(Vector2 position)
{
	using namespace Constants;

	const float halfSize = CellSize / 2.5f;
	Vector2     arr[]    = {{0,         0},
	                        {halfSize,  0},
	                        {0,         halfSize},
	                        {halfSize,  halfSize},
	                        {-halfSize, 0},
	                        {0,         -halfSize},
	                        {-halfSize, -halfSize},
	                        {halfSize,  -halfSize},
	                        {-halfSize, halfSize}};

	for ( auto i: arr )
	{
		auto pos = Vector2Add(position, i);
		pos.x = floor(pos.x / CellSize);
		pos.y = floor(pos.y / CellSize);

		std::cout << pos.x << " " << pos.y << "\n";
		if ( pos.x < 0 )
		{
			pos.x = CellsX - 1;
		}
		else if ( pos.x >= CellsX )
		{
			pos.x = 0;
		}

		if ( pos.y < 0 )
		{
			pos.y = CellsY - 1;
		}
		else if ( pos.y >= CellsY )
		{
			pos.y = 0;
		}

		Vector2 start = {pos.x * CellSize, pos.y * CellSize};
		DrawRectangleV(start, {CellSize, CellSize}, RED);
	}
}

int main()
{
	InitWindow(Constants::ScreenWidth, Constants::ScreenHeight, "Title");

	std::vector<Microbe> microbes(5);

	TextureHandler textureHandler;
	textureHandler.LoadTexture("../assets/microbe_texture.png");

	std::cout << "X:" << Constants::CellsX << " Y:" << Constants::CellsY << std::endl;

	double delay = 5.f, lastUpdate = 0.f;
	bool   flip  = false;
	while ( !WindowShouldClose())
	{
		for ( auto &microbe: microbes )
		{
			microbe.Update(GetFrameTime());
		}

		if ( GetTime() >= lastUpdate + delay )
		{
			DEBUG_LOG_INFO("UPDATE");
			lastUpdate = GetTime();

			for ( auto &microbe: microbes )
			{
				microbe.ChangeDirection();
			}
		}

		BeginDrawing();

		ClearBackground(WHITE);

		DrawGrid();

		for ( auto &microbe: microbes )
		{
			//DrawOnGrid(microbe.GetPos());
			microbe.Draw();
		}

		DrawFPS(0, 0);


		EndDrawing();

		if ( IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			auto mousePos = GetMousePosition();

			DrawOnGrid(mousePos);
		}

		if ( IsKeyPressed(KEY_SPACE))
		{
			for ( auto &microbe: microbes )
			{
				microbe.SetShouldMove(flip);
			}
			flip = !flip;
		}

	}
	CloseWindow();

	return 0;
}
