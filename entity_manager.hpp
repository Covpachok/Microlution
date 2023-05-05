//
// Created by heylc on 26.04.2023.
//

#ifndef MICROLUTION_ENTITY_MANAGER_HPP
#define MICROLUTION_ENTITY_MANAGER_HPP

#include <list>
#include <memory>
//#include "entity.hpp"
#include "stuff/timer.hpp"
//#include "microbe.hpp"
//#include "food.hpp"
//#include "grid.hpp"

class Microbe;

class Food;

class Entity;

class Grid;

class EntityManager
{
	static EntityManager *mInstance;

public:
	using EntityList = std::list<std::shared_ptr<Entity>>;

public:
	EntityManager();

	void Update(float delta, Grid &grid);

	void SpawnMicrobe(Microbe *newMicrobe);

	static EntityManager &GetInstance() { return *mInstance; }

	EntityList &GetEntityList() { return mEntities; }

	size_t GetVegetableCount() const { return mVegetableCount; }
	size_t GetMeatCount() const { return mMeatCount; }
	size_t GetHerbivorousCount() const { return mHerbivorousCount; }
	size_t GetPredatorCount() const { return mPredatorCount; }

private:
	void SpawnVegetables(size_t amount); // randomly
	void SpawnMeat(Entity *from);
	void DeadEntitiesCleanup();
	void DestroyEntities(); // destroys only dead entities, and spawns meat on their places

private:
	EntityList mEntities;
//	FoodList mFood;

//	MicrobeList mMicrobesPool;
//	FoodList mFoodPool;

	size_t mVegetablesSpawnAmount;

	size_t mVegetableCount = 0;
	size_t mMeatCount      = 0;

	size_t mHerbivorousCount = 0;
	size_t mPredatorCount    = 0;

	Timer mSpawnVegetablesTimer;
};


#endif //MICROLUTION_ENTITY_MANAGER_HPP
