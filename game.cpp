//
// Created by heylc on 11.04.2023.
//

#include "game.hpp"
#include "raylib.h"
#include "utils.hpp"
#include "microbe.hpp"
#include "logger.hpp"

Game::Game() :
		mEntities(50)
{
	InitWindow(Constants::ScreenWidth, Constants::ScreenHeight, "Microlution");
	for ( auto &entityPtr: mEntities )
	{
		entityPtr = std::shared_ptr<Entity>(new Microbe());
	}

	/* TEMPORARY CODE, REPLACE IT */
	mTextureHandler.LoadTexture("../assets/microbe_texture.png");

	mGrid.RegisterEntities(mEntities);
}

Game::~Game()
{
	CloseWindow();
}

/** HELPER FUNCTIONS */
void DrawGrid()
{
	Color        color = {0, 0, 0, 16};
	for ( size_t x     = 0; x < Constants::GridWidth; ++x )
	{
		size_t startX = x * Constants::CellSize;
		DrawLine(startX, 0, startX, Constants::ScreenHeight, color);
	}

	for ( size_t y = 0; y < Constants::GridHeight; ++y )
	{
		size_t startY = y * Constants::CellSize;
		DrawLine(0, startY, Constants::ScreenWidth, startY, color);
	}
}

void DrawOnGrid(Vector2 position)
{
	using namespace Constants;

	const float   halfSize = CellSize / 2.5f;
	const Vector2 arr[]    = {{0,         0},
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

		if ( pos.x < 0 )
		{
			pos.x = GridWidth - 1;
		}
		else if ( pos.x >= GridWidth )
		{
			pos.x = 0;
		}

		if ( pos.y < 0 )
		{
			pos.y = GridHeight - 1;
		}
		else if ( pos.y >= GridHeight )
		{
			pos.y = 0;
		}

		Vector2 start = {pos.x * CellSize, pos.y * CellSize};
		DrawRectangleV(start, {CellSize, CellSize}, RED);
	}
}
/** HELPER FUNCTIONS END */


void Game::Start()
{
	while ( !WindowShouldClose())
	{
		Update();
		Draw();
		HandleInput();
	}
}

void Game::HandleInput()
{

}

void Game::Update()
{
	const double  delay      = 2;
	static double lastUpdate = 0;

	for ( auto &microbe: mEntities )
	{
//		microbe->Update(GetFrameTime());
	}

	if ( GetTime() >= lastUpdate + delay )
	{
		DEBUG_LOG_INFO("UPDATE");
		lastUpdate = GetTime();

		for ( auto &microbe: mEntities )
		{
			//microbe->ChangeDirection();
		}
	}
}

void Game::Draw()
{
	BeginDrawing();

	ClearBackground(WHITE);

	mGrid.Draw();
	for ( auto &microbe: mEntities )
	{
//		DrawOnGrid(microbe->GetPos());
	}

	DrawGrid();

	for ( auto &microbe: mEntities )
	{
		microbe->Draw();
	}


	DrawFPS(0, 0);

	EndDrawing();
}