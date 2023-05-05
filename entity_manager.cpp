//
// Created by heylc on 26.04.2023.
//

#include "entity_manager.hpp"
#include <cassert>
#include "entity.hpp"
#include "microbe.hpp"
#include "food.hpp"
#include "grid.hpp"
#include "constants.hpp"

EntityManager *EntityManager::mInstance = nullptr;

const size_t kVegetablesSpawnAmount  = 20;
const size_t kHerbivorousSpawnAmount = 40;
const size_t kPredatorSpawnAmount    = 8;

EntityManager::EntityManager()
{
	assert(mInstance == nullptr);
	mInstance = this;

	mSpawnVegetablesTimer.SetDelay(10);
	SpawnVegetables(kVegetablesSpawnAmount);

	for ( size_t i = 0; i < kPredatorSpawnAmount; ++i )
	{
		SpawnMicrobe(new Microbe(Entity::ePredator));
	}

	for ( size_t i = 0; i < kHerbivorousSpawnAmount; ++i )
	{
		SpawnMicrobe(new Microbe(Entity::eHerbivorous));
	}

	mVegetablesSpawnAmount = mHerbivorousCount / 2;
}

void EntityManager::Update(float delta, Grid &grid)
{
	mSpawnVegetablesTimer.Update(delta);
	if ( mSpawnVegetablesTimer.IsElapsed())
	{
		SpawnVegetables(mVegetablesSpawnAmount);
		mSpawnVegetablesTimer.Reset();
	}

	for ( auto &entity: mEntities )
	{
		if ( entity->IsDead())
		{
			continue;
		}

		entity->Update(delta);
	}

	grid.RegisterEntities(mEntities);

	for ( auto &entity: mEntities )
	{
		if ( entity->IsDead())
		{
			continue;
		}
		grid.CheckCollision(entity.get());
	}

	DeadEntitiesCleanup();

	mVegetablesSpawnAmount = mHerbivorousCount / 2;
}

void EntityManager::SpawnMicrobe(Microbe *newMicrobe)
{
	auto type = newMicrobe->GetType();
	if ( type == Entity::eHerbivorous )
	{
		++mHerbivorousCount;
	}
	else if ( type == Entity::ePredator )
	{
		++mPredatorCount;
	}

	mEntities.push_back(std::shared_ptr<Entity>(newMicrobe));
}

void EntityManager::SpawnVegetables(size_t amount)
{
	mVegetableCount += amount;
	for ( size_t i = 0; i < amount; ++i )
	{
		mEntities.push_back(std::shared_ptr<Entity>(new Food()));
	}
}

void EntityManager::SpawnMeat(Entity *from)
{
	++mMeatCount;
	mEntities.push_back(std::shared_ptr<Entity>(new Food(Entity::eMeat, from->GetPos(), from->GetNutritionValue())));
}

void EntityManager::DeadEntitiesCleanup()
{
	auto       toDelete = mEntities.end();
	for ( auto i        = mEntities.begin(); i != mEntities.end(); )
	{
		toDelete = i;
		auto entity = *toDelete;
		++i;
		if ( entity->IsDead())
		{
			switch ( entity->GetType())
			{
				case Entity::ePredator:
					SpawnMeat(entity.get());
					--mPredatorCount;
					break;
				case Entity::eHerbivorous:
					SpawnMeat(entity.get());
					--mHerbivorousCount;
					break;
				case Entity::eVegetable:
					--mVegetableCount;
					break;
				case Entity::eMeat:
					--mMeatCount;
					break;
				default:
					break;
			}
			mEntities.erase(toDelete);
		}
	}

}
