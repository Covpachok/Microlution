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

private:
	void SpawnVegetables(); // randomly
	void SpawnMeat(/*on position*/);
	void DestroyEntities(); // destroys only dead entities, and spawns meat on their places

private:
	EntityList mEntities;
//	FoodList mFood;

//	MicrobeList mMicrobesPool;
//	FoodList mFoodPool;

	int mVegetableCount = 0;
	int mMeatCount      = 0;

	int mHerbivorousCount = 0;
	int mPredatorCount    = 0;

	Timer mSpawnVegetables;
};


#endif //MICROLUTION_ENTITY_MANAGER_HPP
