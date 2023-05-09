#ifndef MICROLUTION_GRID_HPP
#define MICROLUTION_GRID_HPP

#include <raylib.h>
#include <cmath>
#include <memory>
#include <vector>
#include <set>
#include <list>

#include "stuff/utils.hpp"
#include "stuff/intvec.hpp"
#include "constants.hpp"

class Entity;

class Grid
{
public:
	using EntitySet = std::set<Entity *>;

public:
	Grid();

	// RegisterEntity only one at a time
	void RegisterEntities(std::list<std::shared_ptr<Entity>> &entities);
	void RegisterEntity(Entity *entity);

	void ClearRegisteredEntities();

	const EntitySet &GetEntities(const Vector2 &pos);
	void CheckCollision(Entity *checkEntity);

	void Draw();

private:
	static IntVec2 FitOnGrid(IntVec2 pos);
	static IntVec2 ToGridPosition(const Vector2 &pos) { return ToGridPosition(pos.x, pos.y); }
	static IntVec2 ToGridPosition(float x, float y) { return {FloorToGridUnits(x), FloorToGridUnits(y)}; }

	static int FloorToGridUnits(float n) { return static_cast<int>(std::floor(n / Constants::CellSize)); }
	static int CeilToGridUnits(float n) { return static_cast<int>(std::ceil(n / Constants::CellSize)); }

private:
	std::array<std::array<EntitySet, Constants::GridWidth>, Constants::GridHeight> mGrid;
};


#endif //MICROLUTION_GRID_HPP
