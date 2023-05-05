//
// Created by heylc on 11.04.2023.
//

#ifndef MICROLUTION_GAME_HPP
#define MICROLUTION_GAME_HPP

#include <list>
#include <memory>
#include "stuff/utils.hpp"
#include "grid.hpp"
#include "entity.hpp"
#include "stuff/timer.hpp"
#include "microbe.hpp"
#include "food.hpp"
#include "entity_manager.hpp"

/*
 * I don't know how to make collision detection properly, so
 * each tick grid will be updated, all entities will be registered on it,
 * then grid will check collisions for each entity one by one, each entity
 * colliding with the other one will receive a message, they will process this
 * message and do something, like do a fight or breed, but grid will not be updated,
 * all entities will still remain on the grid, yet some of them may be dead, so we
 * will not process them, childrens will be added into a queue to be processed after
 * collision detection, so they will be created after, also dead entities will be
 * deleted from the entity list and grid.
 *
 * Entity update -> Entity registration on the grid -> Collision checks -> Childs spawning -> Dead entities disposal
 *
 * Collision checks >---collide msg>---> Other microbe ? fight or breed: eat food if it suits
 * if breed -> takes both entities stats and combine them, sends a request to the Game class to add making entity in queue
 * if fight -> do fight, dead entities will set flag *dead*, so they will not be processed by grid, dispose them after
 */

/*
 * Entity manager stores microbes and food separately, then it passes all the entities to the grid one by one.
 * Grid register all of them as the ENTITIES, grid shouldn't know anything about microbes or food types, it only aware
 * of that it is the entity and it is on the grid.
 */

class Game
{
public:
	Game();
	~Game();

	void Start();

private:
	void HandleInput();
	void Update();
	void Draw();

private:
//	void SpawnMicrobes(size_t amount);
//	void SpawnFood(size_t amount);
//	void BreedMicrobes(const Entity &first, const Entity &second);

private:
//	std::list<std::shared_ptr<Entity>> mEntities;

	EntityManager mEntityManager;

	Grid mGrid;

	TextureHandler mTextureHandler;

	bool mPause;

	float mGameSpeed = 1.f;
};


#endif //MICROLUTION_GAME_HPP
