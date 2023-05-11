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

	// Meat can spawn only when microbe dies, also it have nutrition value of dead microbe
	void SpawnMeat(Entity *from);

	// Destroys dead entities
	void DeadEntitiesCleanup();

	std::string GetStatisticsString() const;

	void Reset();

private:
	// Randomly spawns vegetable
	void SpawnVegetables(size_t amount);

private:
	EntityList mEntities;

	size_t mVegetablesSpawnAmount;

	size_t mVegetableCount   = 0;
	size_t mMeatCount        = 0;
	size_t mHerbivorousCount = 0;
	size_t mPredatorCount    = 0;

	size_t mPredatorsDiedCount   = 0;
	size_t mHerbivorousDiedCount = 0;

	Timer mSpawnVegetablesTimer;
};


#endif //MICROLUTION_ENTITY_MANAGER_HPP
