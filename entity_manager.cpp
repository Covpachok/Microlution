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

EntityManager::EntityManager()
{
	assert(mInstance == nullptr);
	mInstance = this;

	for ( int i = 0; i < Constants::MicrobesAmount; ++i )
	{
		mEntities.push_back(std::shared_ptr<Entity>(new Microbe()));
	}
}

void EntityManager::Update(float delta, Grid &grid)
{
	for ( auto &entity: mEntities )
	{
		entity->Update(delta);
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

	auto       toDelete = mEntities.end();
	for ( auto i        = mEntities.begin(); i != mEntities.end(); )
	{
		toDelete = i;
		auto entity = *toDelete;
		++i;
		if ( entity->IsDead())
		{
			if ( entity->GetType() == Entity::eHerbivorous || entity->GetType() == Entity::ePredator )
			{
				mEntities.push_back(std::shared_ptr<Entity>(new Food(Entity::eMeat, entity->GetPos())));
			}
			mEntities.erase(toDelete);
		}
	}
}

void EntityManager::SpawnMicrobe(Microbe *newMicrobe)
{
	mEntities.push_back(std::shared_ptr<Entity>(newMicrobe));
}


//void EntityManager::AddEntity(Entity *newEntity)
//{
//
//}
