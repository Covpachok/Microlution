//
// Created by heylc on 12.04.2023.
//

#ifndef MICROLUTION_GRID_HPP
#define MICROLUTION_GRID_HPP

#include <memory>
#include <list>
#include <vector>
#include "utils.hpp"

class Entity;

class Grid
{
public:
	Grid();

	void RegisterEntities(std::vector<std::shared_ptr<Entity>>& entities);
	void CheckCollisions(size_t x, size_t y, size_t id);

	void Draw();

private:
	Array2D<std::vector<Entity*>> mGrid;
};


#endif //MICROLUTION_GRID_HPP
