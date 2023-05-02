//
// Created by heylc on 12.04.2023.
//

#include "grid.hpp"

#include <cassert>
#include <algorithm>

#include "entity.hpp"
#include "constants.hpp"
#include "stuff/math.hpp"
#include "stuff/logger.hpp"

Grid::Grid()
{
	for ( auto &row: mDebugGrid )
	{
		for ( auto &cell: row )
		{
			cell = false;
		}
	}
}

bool InsideCircle(Vector2 center, Vector2 tile, float radius)
{
	float dx               = center.x - tile.x,
	      dy               = center.y - tile.y;
	float distance_squared = dx * dx + dy * dy;
	return distance_squared <= radius * radius;
}

void Grid::RegisterEntity(Entity *entity)
{
	Vector2 pos      = entity->GetPos();
	float   bodySize = entity->GetBodySize();

	Vector2   convPos = {pos.x / Constants::CellSize, pos.y / Constants::CellSize};
	int       size    = round(bodySize);
	for ( int y       = -size; y <= size; ++y )
	{
		for ( int x = -size; x <= size; ++x )
		{
			IntVec2 gridPos  = ToGridPosition(pos) + IntVec2{x, y};
			Vector2 relative = {floor(convPos.x + x) + 0.5f,
			                    floor(convPos.y + y) + 0.5f};
			Vector2 center   = convPos;
			if ( InsideCircle(center, relative, bodySize))
			{
				int nx = std::min(std::max(gridPos.x, 0), (int) Constants::GridWidth - 1);
				int ny = std::min(std::max(gridPos.y, 0), (int) Constants::GridHeight - 1);

				mGrid[ny][nx].insert(entity);
			}
		}
	}
}

void Grid::RegisterEntities(std::list<std::shared_ptr<Entity>> &entities)
{
	ClearRegisteredEntities();
	for ( auto &entity: entities )
	{
		RegisterEntity(entity.get());

#if 0
		const float bodyRadius = entity->GetBodyRadius();
		int         bodySize     = 1;

		Vector2 offset;
		IntVec2 gridPos;
		size_t  gridX, gridY;

		for ( int y = -bodySize; y <= bodySize; ++y )
		{
			for ( int x = -bodySize; x <= bodySize; ++x )
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
#endif

	}
}

const Grid::EntitySet &Grid::GetEntities(const Vector2 &pos)
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

void Grid::CheckCollision(Entity *checkEntity)
{
	IntVec2 cellPos    = ToGridPosition(checkEntity->GetPos());
	int     cellRadius = ceil(checkEntity->GetPerceptionRadius());

	std::list<Entity *> ret;
	IntVec2             curr;

	for ( int y = -cellRadius; y <= cellRadius; ++y )
	{
		for ( int x = -cellRadius; x <= cellRadius; ++x )
		{
			curr = {cellPos.x + x, cellPos.y + y};
			curr = FitOnGrid(curr);
			for ( auto &otherEntity: mGrid[curr.y][curr.x] )
			{
				if ( otherEntity == checkEntity || otherEntity == nullptr || otherEntity->IsDead())
				{
					continue;
				}

				if ( checkEntity->GetPerceptionRadius() > 0 &&
				     CheckCollisionCircles(otherEntity->GetPos(), otherEntity->GetBodyRadius() * Constants::CellSize,
				                           checkEntity->GetPos(),
				                           checkEntity->GetPerceptionRadius() * Constants::CellSize))
				{
					checkEntity->OnPerceptionCollisionEnter(*otherEntity);
				}

				if ( CheckCollisionCircles(otherEntity->GetPos(), otherEntity->GetBodyRadius() * Constants::CellSize,
				                           checkEntity->GetPos(),
				                           checkEntity->GetBodyRadius() * Constants::CellSize))
				{
					checkEntity->OnBodyCollisionEnter(*otherEntity);
				}
			}
		}
	}
}

void Grid::Draw()
{
	for ( size_t x = 0; x < Constants::GridWidth; ++x )
	{
		size_t startX = x * Constants::CellSize;
		DrawLine(startX, 0, startX, Constants::ScreenHeight, {0, 0, 0, 16});
	}

	for ( size_t y = 0; y < Constants::GridHeight; ++y )
	{
		size_t startY = y * Constants::CellSize;
		DrawLine(0, startY, Constants::ScreenWidth, startY, {0, 0, 0, 16});
	}

	for ( size_t y = 0; y < mGrid.size(); ++y )
	{
		for ( size_t x = 0; x < mGrid[y].size(); ++x )
		{
			const float size  = Constants::CellSize;
			Vector2     start = {x * size, y * size};

			if ( mDebugGrid[y][x] )
			{
				DrawRectangleV(start, {size, size}, MAGENTA);
			}

			if ( mGrid[y][x].empty())
			{
				continue;
			}

			DrawRectangleV(start, {size, size}, RED);
		}
	}
}

void Grid::ClearRegisteredEntities()
{
	for ( auto &row: mGrid )
	{
		for ( auto &set: row )
		{
			set.clear();
		}
	}
}

IntVec2 Grid::FitOnGrid(IntVec2 pos)
{
#if 0
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
#endif

	pos.x = std::min(std::max(pos.x, 0), (int) Constants::GridWidth - 1);
	pos.y = std::min(std::max(pos.y, 0), (int) Constants::GridHeight - 1);

	return pos;
}

void Grid::RegisterOnDebugGrid(Vector2 pos, float radius)
{
	for ( auto &row: mDebugGrid )
	{
		for ( auto &cell: row )
		{
			cell = false;
		}
	}

	Vector2   convPos = {pos.x / Constants::CellSize, pos.y / Constants::CellSize};
	int       size    = round(radius);
	for ( int y       = -size; y <= size; ++y )
	{
		for ( int x = -size; x <= size; ++x )
		{
			IntVec2 gridPos  = ToGridPosition(pos) + IntVec2{x, y};
			Vector2 relative = {floor(convPos.x + x) + 0.5f,
			                    floor(convPos.y + y) + 0.5f};
			Vector2 center   = convPos;
			if ( InsideCircle(center, relative, radius))
			{
				int nx = std::min(std::max(gridPos.x, 0), (int) Constants::GridWidth - 1);
				int ny = std::min(std::max(gridPos.y, 0), (int) Constants::GridHeight - 1);

				mDebugGrid[ny][nx] = true;
			}
		}
	}
}
