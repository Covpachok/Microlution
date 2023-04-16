//
// Created by heylc on 11.04.2023.
//

#ifndef MICROLUTION_GAME_HPP
#define MICROLUTION_GAME_HPP

#include <list>
#include <memory>
#include "utils.hpp"
#include "grid.hpp"

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
	std::vector<std::shared_ptr<class Entity>> mEntities;

	Grid mGrid;

	TextureHandler mTextureHandler;
};


#endif //MICROLUTION_GAME_HPP
