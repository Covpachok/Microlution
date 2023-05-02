//
// Created by heylc on 11.04.2023.
//

#include <raylib.h>

#include <memory>

#include "game.hpp"
#include "constants.hpp"
#include "microbe.hpp"
#include "stuff/logger.hpp"
#include "raymath.h"

const size_t gMicrobesAmount = 10;

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

	mEntitySpawnTimer.SetDelay(10);
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

	if ( IsKeyPressed(KEY_ENTER))
	{
//		SpawnMicrobes(100);
	}

	if ( IsKeyPressed(KEY_EQUAL))
	{
		mTEMPRadius += 0.1f;
	}
	if ( IsKeyPressed(KEY_MINUS))
	{
		mTEMPRadius -= 0.1f;
	}

	if ( IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
	{
		mGrid.RegisterOnDebugGrid(GetMousePosition(), mTEMPRadius);
	}
}

void Game::Update()
{
	float delta = GetFrameTime();
	mEntityManager.Update(delta, mGrid);
}

void Game::Draw()
{
	BeginDrawing();

	ClearBackground(WHITE);

//	mGrid.Draw();
	auto &entites = mEntityManager.GetEntityList();

	for ( auto &entity: entites )
	{
		entity->Draw();
	}

	DrawFPS(0, 0);
	DrawText(std::to_string(entites.size()).c_str(), 0, 20, 30, RED);

	EndDrawing();
}