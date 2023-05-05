//
// Created by heylc on 11.04.2023.
//

#include <raylib.h>

#include <memory>

#include "game.hpp"
#include "constants.hpp"
#include "microbe.hpp"
#include "stuff/logger.hpp"
#include "stuff/global.hpp"

using namespace std;

Game::Game() :
		mPause(false)
{
	InitWindow(Constants::ScreenWidth, Constants::ScreenHeight, "Microlution");
//	for ( int i = 0; i < gMicrobesAmount; ++i )
//	{
//		mEntities.push_back(std::shared_ptr<Entity>(new Microbe()));
//	}

//	SpawnFood(10);
//	for ( auto &entityPtr: mEntities )
//	{
//		entityPtr = std::shared_ptr<Entity>(new Microbe());
//	}

	/* TEMPORARY CODE, REPLACE IT */
	mTextureHandler.LoadTexture("../assets/microbe_texture.png");

	mGrid.RegisterEntities(mEntityManager.GetEntityList());
	DEBUG_LOG_INFO(std::to_string(Constants::GridWidth) + " " + std::to_string(Constants::GridHeight));
}

Game::~Game()
{
	CloseWindow();
}

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
	if ( IsKeyDown(KEY_MINUS))
	{
		mGameSpeed = max(mGameSpeed - 0.01f, 0.f);
	}

	if ( IsKeyDown(KEY_EQUAL))
	{
		mGameSpeed += 0.01f;
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

void Game::Update()
{
	float delta = GetFrameTime() * mGameSpeed;
	mEntityManager.Update(delta, mGrid);
}

void Game::Draw()
{
	BeginDrawing();

	ClearBackground(WHITE);

	auto &entites = mEntityManager.GetEntityList();

	if ( GlobalDebug::gDrawDebugGrid )
	{
		mGrid.Draw();
	}

	for ( auto &entity: entites )
	{
		entity->Draw();
	}

	DrawFPS(1, 1);

	std::string debugText =
			            "E:" + to_string(entites.size()) + "\nH:" + to_string(mEntityManager.GetHerbivorousCount()) +
			            "\nP:" +
			            to_string(mEntityManager.GetPredatorCount()) + "\nV:" +
			            to_string(mEntityManager.GetVegetableCount()) + "\nM:" +
			            to_string(mEntityManager.GetMeatCount());

	DrawText(debugText.c_str(), 1, 20, 20, BLACK);

	DrawText(to_string(mGameSpeed).c_str(), Constants::ScreenWidth - 50, 1, 20, BLUE);

	EndDrawing();
}