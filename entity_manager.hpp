#ifndef MICROLUTION_ENTITY_MANAGER_HPP
#define MICROLUTION_ENTITY_MANAGER_HPP

#include <list>
#include <memory>
#include "stuff/timer.hpp"

// Forward declarations
class Microbe;
class Food;
class Entity;
class Grid;

class EntityManager
{
	// Singleton
	static EntityManager *mInstance;

public:
	using EntityList = std::list<std::shared_ptr<Entity>>;

public:
	EntityManager();

	void Update(float delta, Grid &grid);

	// When breeding microbes need to spawn their brood
	void SpawnMicrobe(Microbe *newMicrobe);

	static EntityManager &GetInstance() { return *mInstance; }

	// Game need an EntityList to draw entities on the screen,
	// however EntityManager can also handle this, so idk
	EntityList &GetEntityList() { return mEntities; }

	size_t GetVegetableCount() const { return mVegetableCount; }
	size_t GetMeatCount() const { return mMeatCount; }
	size_t GetHerbivorousCount() const { return mHerbivorousCount; }
	size_t GetPredatorCount() const { return mPredatorCount; }

private:
	// Randomly spawns vegetable
	void SpawnVegetables(size_t amount);

	// Meat can spawn only when microbe dies, also it have nutrition value of dead microbe
	void SpawnMeat(Entity *from);

	// Destroys dead entities
	void DeadEntitiesCleanup();

private:
	EntityList mEntities;

	size_t mVegetablesSpawnAmount;

	size_t mVegetableCount   = 0;
	size_t mMeatCount        = 0;
	size_t mHerbivorousCount = 0;
	size_t mPredatorCount    = 0;

	Timer mSpawnVegetablesTimer;
};


#endif //MICROLUTION_ENTITY_MANAGER_HPP
