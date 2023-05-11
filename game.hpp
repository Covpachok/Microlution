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

	Font mDebugFont;

	class Entity *mDebugChosenEntity = nullptr;

	float mGameSpeed     = 1.f;
	bool  mPause         = false;
	bool  mDrawDebugText = false;
};


#endif //MICROLUTION_GAME_HPP
