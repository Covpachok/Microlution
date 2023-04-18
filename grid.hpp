//
// Created by heylc on 12.04.2023.
//

#ifndef MICROLUTION_GRID_HPP
#define MICROLUTION_GRID_HPP

#include <raylib.h>
#include <cmath>
#include <memory>
#include <vector>
#include <set>
#include <list>

#include "utils.hpp"
#include "intvec.hpp"
#include "constants.hpp"

class Entity;

class Grid
{
public:
	using EntityList = std::set<Entity *>;

public:
	Grid();

	void RegisterEntities(std::vector<std::shared_ptr<Entity>> &entities);

	const EntityList &GetEntities(const Vector2 &pos);
	void CheckPerceptionCollision(Entity *checkEntity);
	void CheckBodyCollision(Entity *checkEntity);

	void Draw();

private:
	std::list<Entity *> GetEntitiesInRadius(const Vector2 &pos, float radius);
	void ClearRegisteredEntities();

	static IntVec2 FitOnGrid(IntVec2 pos);
	static IntVec2 ToGridPosition(const Vector2 &pos) { return ToGridPosition(pos.x, pos.y); }
	static IntVec2 ToGridPosition(float x, float y) { return {FloorToGridUnits(x), FloorToGridUnits(y)}; }
	static int FloorToGridUnits(float n) { return static_cast<int>(std::floor(n / Constants::CellSize)); }
	static int CeilToGridUnits(float n) { return static_cast<int>(std::ceil(n / Constants::CellSize)); }

private:
	Array2D<EntityList> mGrid;
};


#endif //MICROLUTION_GRID_HPP
