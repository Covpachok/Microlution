//
// Created by heylc on 12.04.2023.
//

#include "grid.hpp"

#include <cassert>
#include <algorithm>

#include "entity.hpp"
#include "constants.hpp"
#include "math.hpp"
#include "logger.hpp"

Grid::Grid()
{
	mGrid.resize(Constants::GridHeight);
	assert(mGrid.size() == Constants::GridHeight);
	for ( auto &row: mGrid )
	{
		row.resize(Constants::GridWidth);
		assert(row.size() == Constants::GridWidth);
	}
}

void Grid::RegisterEntities(std::vector<std::shared_ptr<Entity>> &entities)
{
	ClearRegisteredEntities();
	for ( auto &entity: entities )
	{
		const float bodyRadius = entity->GetBodyRadius();
		int         radius     = 1;

		Vector2 offset;
		IntVec2 gridPos;
		size_t  gridX, gridY;

		for ( int y = -radius; y <= radius; ++y )
		{
			for ( int x = -radius; x <= radius; ++x )
			{
				offset  = {static_cast<float>(x) * bodyRadius, static_cast<float>(y) * bodyRadius};
				gridPos = ToGridPosition(Vector2Add(entity->GetPos(), offset));

				if ( gridPos.x < 0 )
				{
					gridPos.x = Constants::GridWidth - 1;
				}
				else if ( gridPos.x >= Constants::GridWidth )
				{
					gridPos.x = 0;
				}

				if ( gridPos.y < 0 )
				{
					gridPos.y = Constants::GridHeight - 1;
				}
				else if ( gridPos.y >= Constants::GridHeight )
				{
					gridPos.y = 0;
				}

				gridX = static_cast<size_t>(gridPos.x);
				gridY = static_cast<size_t>(gridPos.y);

				mGrid[gridY][gridX].insert(entity.get());
			}
		}

	}
}

//void Grid::GetEntities(size_t id)
//{
//	auto entity = mGrid[y][x].at(id);
//}

const Grid::EntityList &Grid::GetEntities(const Vector2 &pos)
{
	IntVec2 gridPos = ToGridPosition(pos);

	int resX = gridPos.x;
	int resY = gridPos.y;

	if ( resX < 0 )
	{
		resX = Constants::GridWidth - 1;
	}
	else if ( resX >= Constants::GridWidth )
	{
		resX = 0;
	}

	if ( resY < 0 )
	{
		resY = Constants::GridHeight - 1;
	}
	else if ( resY >= Constants::GridHeight )
	{
		resY = 0;
	}

	assert(gridPos.y >= 0 && gridPos.y < Constants::GridHeight);
	assert(gridPos.x >= 0 && gridPos.x < Constants::GridWidth);

#if 0
	for ( auto &entity: mGrid[resY][resX] )
	{
		if ( entity == nullptr )
		{
			continue;
		}

		std::cout << "Collided" << "\n";
	}
#endif

	return mGrid[resY][resX];
}

std::list<Entity *> Grid::GetEntitiesInRadius(const Vector2 &pos, float radius)
{
	IntVec2 cellPos    = ToGridPosition(pos);
	int     cellRadius = CeilToGridUnits(radius);

	std::list<Entity *> ret;
	IntVec2             curr;

	for ( int y = -cellRadius; y <= cellRadius; ++y )
	{
		for ( int x = -cellRadius; x <= cellRadius; ++x )
		{
			curr = {cellPos.x + x, cellPos.y + y};
			curr = FitOnGrid(curr);
			for ( auto &entity: mGrid[curr.y][curr.x] )
			{
				ret.push_back(entity);
			}
		}
	}
	return ret;
}

void Grid::CheckPerceptionCollision(Entity *checkEntity)
{
	auto entityInRadius = GetEntitiesInRadius(checkEntity->GetPos(), checkEntity->GetPerceptionRadius());

	for ( auto &otherEntity: entityInRadius )
	{
		if ( otherEntity == checkEntity || otherEntity == nullptr )
		{
			continue;
		}

		if ( Vector2Distance(otherEntity->GetPos(), checkEntity->GetPos()) <= checkEntity->GetPerceptionRadius())
		{
			checkEntity->OnPerceptionCollisionEnter(*otherEntity);
		}
	}

#if 0
	IntVec2 cellPos    = ToGridPosition(checkEntity->GetPos());
	int     cellRadius = CeilToGridUnits(checkEntity->GetPerceptionRadius());

	IntVec2 curr;

	for ( int y = -cellRadius; y <= cellRadius; ++y )
	{
		for ( int x = -cellRadius; x <= cellRadius; ++x )
		{
			curr = {cellPos.x + x, cellPos.y + y};
			curr = FitOnGrid(curr);
			for ( auto &entity: mGrid[curr.y][curr.x] )
			{
				if ( entity == checkEntity || entity == nullptr )
				{
					continue;
				}

				if ( Vector2Distance(entity->GetPos(), checkEntity->GetPos()) <= checkEntity->GetPerceptionRadius())
				{
					checkEntity->OnPerceptionCollisionEnter(*entity);
				}
			}
		}
	}
#endif
}

void Grid::CheckBodyCollision(Entity *checkEntity)
{
	auto entityInRadius = GetEntitiesInRadius(checkEntity->GetPos(), checkEntity->GetBodyRadius());

	for ( auto &otherEntity: entityInRadius )
	{
		if ( otherEntity == checkEntity || otherEntity == nullptr )
		{
			continue;
		}

		if ( Vector2Distance(otherEntity->GetPos(), checkEntity->GetPos()) <= checkEntity->GetBodyRadius())
		{
			checkEntity->OnBodyCollisionEnter(*otherEntity);
		}
	}
}


void Grid::Draw()
{
	for ( size_t y = 0; y < mGrid.size(); ++y )
	{
		for ( size_t x = 0; x < mGrid[y].size(); ++x )
		{
			if ( mGrid[y][x].empty())
			{
				continue;
			}

			const float size  = Constants::CellSize;
			Vector2     start = {x * size, y * size};
			DrawRectangleV(start, {size, size}, RED);
		}
	}
}

void Grid::ClearRegisteredEntities()
{
	for ( auto &row: mGrid )
	{
		for ( auto &list: row )
		{
			list.clear();
		}
	}
}
IntVec2 Grid::FitOnGrid(IntVec2 pos)
{
	if ( pos.x < 0 )
	{
		pos.x = Constants::GridWidth - 1;
	}
	else if ( pos.x >= Constants::GridWidth )
	{
		pos.x = 0;
	}

	if ( pos.y < 0 )
	{
		pos.y = Constants::GridHeight - 1;
	}
	else if ( pos.y >= Constants::GridHeight )
	{
		pos.y = 0;
	}

	return pos;
}

