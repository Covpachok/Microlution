//
// Created by heylc on 12.04.2023.
//

#ifndef MICROLUTION_GRID_HPP
#define MICROLUTION_GRID_HPP

#include <memory>
#include <list>
#include <vector>
#include "utils.hpp"
#include "intvec.hpp"

class Entity;

class Grid
{
public:
	Grid();

	void RegisterEntities(std::vector<std::shared_ptr<Entity>> &entities);
//	void CheckCollisions(size_t id);
	void CheckCollisions(Vector2 pos, float radius);

	void Draw();

private:
	void ClearRegisteredEntites();

	static IntVec2 ToGridPosition(Vector2 pos);
	static IntVec2 ToGridPosition(float x, float y);
	static size_t ToGridUnits(float n);

private:
	Array2D<std::vector<Entity *>> mGrid;
};


#endif //MICROLUTION_GRID_HPP
