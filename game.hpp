//
// Created by heylc on 11.04.2023.
//

#ifndef MICROLUTION_GAME_HPP
#define MICROLUTION_GAME_HPP

#include <list>
#include <memory>
#include "utils.hpp"
#include "grid.hpp"
#include "entity.hpp"

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
	void SpawnEntity(Entity::Type type);
	void SpawnEntity(const Entity &first, const Entity &second);

private:
	std::vector<std::shared_ptr<Entity>> mEntities;

	Grid mGrid;

	TextureHandler mTextureHandler;

	bool mPause;
};


#endif //MICROLUTION_GAME_HPP
