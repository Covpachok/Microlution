//
// Created by heylc on 11.04.2023.
//

#include <raylib.h>

#include "game.hpp"
#include "constants.hpp"
#include "microbe.hpp"
#include "logger.hpp"

Game::Game() :
		mEntities(36), mPause(false)
{
	InitWindow(Constants::ScreenWidth, Constants::ScreenHeight, "Microlution");
	for ( auto &entityPtr: mEntities )
	{
		entityPtr = std::shared_ptr<Entity>(new Microbe());
	}

	/* TEMPORARY CODE, REPLACE IT */
	mTextureHandler.LoadTexture("../assets/microbe_texture.png");

	mGrid.RegisterEntities(mEntities);
	DEBUG_LOG_INFO(std::to_string(Constants::GridWidth) + " " + std::to_string(Constants::GridHeight));
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

#if 0
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
#endif
/** HELPER FUNCTIONS END */


void Game::Start()
{
	while ( !WindowShouldClose())
	{
		if ( !mPause )
		{
			Update();
		}
		Draw();
		HandleInput();
	}
}

void Game::HandleInput()
{
	if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		auto       entities = mGrid.GetEntities(GetMousePosition());
		for ( auto &entity: entities )
		{
			if ( entity == nullptr )
			{
				continue;
			}
			DEBUG_LOG_INFO(entity->ToString());
		}
	}

	if ( IsKeyPressed(KEY_SPACE))
	{
		mPause = !mPause;
	}
}

void Game::Update()
{
	for ( auto &entity: mEntities )
	{
		entity->Update(GetFrameTime());
	}

	mGrid.RegisterEntities(mEntities);

	for ( auto &entity: mEntities )
	{
		mGrid.CheckPerceptionCollision(entity.get());
		mGrid.CheckBodyCollision(entity.get());
	}

#if 0
	if ( GetTime() >= lastUpdate + delay )
	{
		DEBUG_LOG_INFO("UPDATE");
		lastUpdate = GetTime();

		for ( auto &microbe: mEntities )
		{
			//microbe->ChangeDirection();
		}
	}
#endif
}

void Game::Draw()
{
	BeginDrawing();

	ClearBackground(WHITE);

//	mGrid.Draw();
//	DrawGrid();

	for ( auto &microbe: mEntities )
	{
		microbe->Draw();
	}


	DrawFPS(0, 0);

	EndDrawing();
}

void Game::SpawnEntity(Entity::Type type)
{
	// Primarily used for initialisation and spawning of vegetables

	// create corresponding entity
	// add to entity list
}

void Game::SpawnEntity(const Entity &first, const Entity &second)
{
	// first and second are parents, children must be spawned adjacent to them, and have similar stats
	// add to entity list
}
