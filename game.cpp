#include <raylib.h>

#include <memory>

#include "game.hpp"
#include "constants.hpp"
#include "microbe.hpp"
#include "stuff/logger.hpp"
#include "stuff/global.hpp"

using namespace std;

const float kFixedTimestep = 2.0 / 60.0;

Game::Game() :
		mPause(false)
{
	using namespace Constants;

	InitWindow(ScreenWidth, ScreenHeight, "Microlution");
	SetTargetFPS(60);

	mTextureHandler.LoadTexture("../assets/microbe_texture.png");

	mDebugFont = LoadFontEx("../assets/monogram.ttf", 128, NULL, 0);
	if ( mDebugFont.glyphCount <= 0 )
	{
		mDebugFont = GetFontDefault();
	}
}

Game::~Game()
{
	CloseWindow();
}

void Game::Start()
{
	float delta = 0;
	while ( !WindowShouldClose())
	{
		Draw();
		HandleInput();
		if ( !mPause )
		{
			delta += GetFrameTime() * mGameSpeed;
			while ( delta >= kFixedTimestep )
			{
				delta -= kFixedTimestep;
				Update(kFixedTimestep);
			}
		}
	}
}

void Game::HandleInput()
{
	if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		auto       entities = mGrid.GetEntities(GetMousePosition());
		for ( auto &entity: entities )
		{
			if ( entity == nullptr ||
			     !( entity->GetType() == Entity::ePredator || entity->GetType() == Entity::eHerbivorous ))
			{
				continue;
			}
			mDebugChosenEntity = entity;
			DEBUG_LOG_INFO(mDebugChosenEntity->ToString());
		}
	}

	if ( IsKeyPressed(KEY_SPACE))
	{
		mPause = !mPause;
	}

	if ( IsKeyPressed(KEY_GRAVE))
	{
		mDrawDebugText = !mDrawDebugText;
	}

	if ( IsKeyPressed(KEY_DELETE))
	{
		mEntityManager.Reset();
		mGrid.ClearGrid();
		mDebugChosenEntity = nullptr;
	}

	using namespace GlobalDebug;

	if ( IsKeyDown(KEY_LEFT_ALT))
	{
		if ( IsKeyPressed(KEY_G))
		{
			gDrawDebugGrid = !gDrawDebugGrid;
		}

		if ( IsKeyPressed(KEY_P))
		{
			gDrawDebugPerceptionRadius = !gDrawDebugPerceptionRadius;
		}

		if ( IsKeyPressed(KEY_D))
		{
			gDrawDebugDirection = !gDrawDebugDirection;
		}
	}

	/* SPEED */
	if ( IsKeyPressed(KEY_MINUS))
	{
		mGameSpeed = std::max(mGameSpeed - 0.5f, 0.f);
	}

	if ( IsKeyPressed(KEY_EQUAL))
	{
		mGameSpeed = std::min(mGameSpeed + 0.5f, 10.f);
	}

	if ( IsKeyPressed(KEY_ZERO))
	{
		mGameSpeed = 1;
	}

	/* Microbe spawning */
	if ( IsKeyDown(KEY_LEFT_CONTROL))
	{
		if ( IsKeyDown(KEY_LEFT_SHIFT))
		{
			if ( IsKeyPressed(KEY_H))
			{
				for ( size_t i = 0; i < 10; ++i )
				{
					mEntityManager.SpawnMicrobe(new Microbe(Entity::eHerbivorous));
				}
			}

			if ( IsKeyPressed(KEY_P))
			{
				for ( size_t i = 0; i < 10; ++i )
				{
					mEntityManager.SpawnMicrobe(new Microbe(Entity::ePredator));
				}
			}
		}
		else
		{
			if ( IsKeyPressed(KEY_H))
			{
				mEntityManager.SpawnMicrobe(new Microbe(Entity::eHerbivorous));
			}

			if ( IsKeyPressed(KEY_P))
			{
				mEntityManager.SpawnMicrobe(new Microbe(Entity::ePredator));
			}
		}
	}
}

void Game::Update(float delta)
{
	mEntityManager.Update(delta, mGrid);

	if ( mDebugChosenEntity != nullptr && mDebugChosenEntity->IsDead())
	{
		mDebugChosenEntity = nullptr;
	}

	mEntityManager.DeadEntitiesCleanup();
}

void Game::Draw()
{
	BeginDrawing();

	ClearBackground(WHITE);

	if ( GlobalDebug::gDrawDebugGrid )
	{
		mGrid.Draw();
	}

	auto &entityList = mEntityManager.GetEntityList();

	for ( auto &entity: entityList )
	{
		entity->Draw();
	}

	if ( mDrawDebugText )
	{
		DrawTextEx(mDebugFont, mEntityManager.GetStatisticsString().c_str(), {1, 20}, 25, 1, BLACK);

		if ( mDebugChosenEntity != nullptr )
		{
			DrawTextEx(mDebugFont, mDebugChosenEntity->ToString().c_str(), {1, Constants::ScreenHeight / 2 - 50}, 25, 1,
			           {200, 0, 200, 255});
		}
	}

	std::string gameSpeedText = TextFormat("%.1f", mGameSpeed);
	DrawText(gameSpeedText.c_str(), Constants::ScreenWidth - MeasureText(gameSpeedText.c_str(), 25), 1, 20, BLUE);
	DrawFPS(1, 1);

	EndDrawing();
}