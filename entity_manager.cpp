#include "entity_manager.hpp"
#include <cassert>
#include "entity.hpp"
#include "microbe.hpp"
#include "food.hpp"
#include "grid.hpp"
#include "constants.hpp"

EntityManager *EntityManager::mInstance = nullptr;

const size_t kVegetablesSpawnAmount  = 300;
const size_t kHerbivorousSpawnAmount = 64;
const size_t kPredatorSpawnAmount    = 8;

const float kSpawnVegetableDelay = 8;

EntityManager::EntityManager()
{
	assert(mInstance == nullptr);
	mInstance = this;

	mSpawnVegetablesTimer.SetDelay(kSpawnVegetableDelay);
	SpawnVegetables(kVegetablesSpawnAmount);
	mVegetablesSpawnAmount = kVegetablesSpawnAmount;

	for ( size_t i = 0; i < kPredatorSpawnAmount; ++i )
	{
		SpawnMicrobe(new Microbe(Entity::ePredator));
	}

	for ( size_t i = 0; i < kHerbivorousSpawnAmount; ++i )
	{
		SpawnMicrobe(new Microbe(Entity::eHerbivorous));
	}
}

void EntityManager::Update(float delta, Grid &grid)
{
	// Spawning vegetables when timer allows to
	mSpawnVegetablesTimer.Update(delta);
	if ( mSpawnVegetablesTimer.IsElapsed())
	{
		SpawnVegetables(mVegetablesSpawnAmount);
		mSpawnVegetablesTimer.Reset();
	}

	// Updating all alive entities
	for ( auto &entity: mEntities )
	{
		if ( entity->IsDead())
		{
			continue;
		}

		entity->Update(delta);
	}

	// Grid handles collisions, so we need to register entities on it
	grid.RegisterEntities(mEntities);

	// Checking collisions of all the entities
	for ( auto &entity: mEntities )
	{
		if ( entity->IsDead())
		{
			continue;
		}
		grid.CheckCollision(entity.get());
	}

	DeadEntitiesCleanup();

	// Better to spawn constant amount of vegetables
//	mVegetablesSpawnAmount = static_cast<int>(std::ceil(static_cast<float>(mHerbivorousCount) / 2.f));
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
		mEntities.push_front(std::shared_ptr<Entity>(new Food()));
	}
}

void EntityManager::SpawnMeat(Entity *from)
{
	++mMeatCount;
	mEntities.push_front(std::shared_ptr<Entity>(new Food(Entity::eMeat, from->GetPos(), from->GetNutritionValue())));
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
