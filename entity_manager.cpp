#include "entity_manager.hpp"
#include <cassert>
#include "entity.hpp"
#include "microbe.hpp"
#include "food.hpp"
#include "grid.hpp"
#include "constants.hpp"

EntityManager *EntityManager::mInstance = nullptr;

const size_t kVegetablesSpawnAmount  = 200;
const size_t kHerbivorousSpawnAmount = 400;
const size_t kPredatorSpawnAmount    = 60;

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
}

void EntityManager::SpawnMicrobe(Microbe *newMicrobe)
{
	assert(newMicrobe != nullptr);

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
	assert(from != nullptr);

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
					--mPredatorCount;
					++mPredatorsDiedCount;
					break;
				case Entity::eHerbivorous:
					--mHerbivorousCount;
					++mHerbivorousDiedCount;
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

std::string EntityManager::GetStatisticsString() const
{
	std::string result;
	result += "#ALIVE";
	result += "\n\tPREDATORS  : " + std::to_string(mPredatorCount);
	result += "\n\tHERBIVOROUS: " + std::to_string(mHerbivorousCount);
	result += "\n\tMEAT       : " + std::to_string(mMeatCount);
	result += "\n\tVEGETABLES : " + std::to_string(mVegetableCount);
	result += "\n#DIED";
	result += "\n\tPREDATORS  : " + std::to_string(mPredatorsDiedCount);
	result += "\n\tHERBIVOROUS: " + std::to_string(mHerbivorousDiedCount);

	return result;
}

void EntityManager::Reset()
{
	mSpawnVegetablesTimer.Reset();

	mVegetableCount       = 0;
	mMeatCount            = 0;
	mHerbivorousCount     = 0;
	mPredatorCount        = 0;
	mHerbivorousDiedCount = 0;
	mPredatorsDiedCount   = 0;

	mEntities.clear();

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
