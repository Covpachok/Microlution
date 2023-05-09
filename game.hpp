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
	EntityManager mEntityManager;

	Grid mGrid;

	TextureHandler mTextureHandler;

	bool mPause;

	float mGameSpeed = 1.f;
};


#endif //MICROLUTION_GAME_HPP
