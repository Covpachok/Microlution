#include "grid.hpp"

#include <cassert>
#include <algorithm>

#include "entity.hpp"
#include "constants.hpp"
#include "stuff/math.hpp"

Grid::Grid() = default;

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
	float   bodySize = entity->GetBodySize(); // Actually is a radius

	Vector2 entityConvertedPos = {pos.x / Constants::CellSize, pos.y / Constants::CellSize};

	int       size = static_cast<int>(round(bodySize));
	for ( int y    = -size; y <= size; ++y )
	{
		for ( int x = -size; x <= size; ++x )
		{
			IntVec2 gridPos = ToGridPosition(pos) + IntVec2{x, y};

			Vector2 relativePos = {floor(entityConvertedPos.x + static_cast<float>(x)) + 0.5f,
			                       floor(entityConvertedPos.y + static_cast<float>(y)) + 0.5f};

			if ( InsideCircle(entityConvertedPos, relativePos, bodySize))
			{
				gridPos = FitOnGrid(gridPos);

				mGrid[gridPos.y][gridPos.x].insert(entity);
			}
		}
	}
}

void Grid::RegisterEntities(std::list<std::shared_ptr<Entity>> &entities)
{
	ClearGrid();
	for ( auto &entity: entities )
	{
		RegisterEntity(entity.get());
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

	return mGrid[resY][resX];
}

void Grid::CheckCollision(Entity *checkEntity)
{
	IntVec2 gridPos    = ToGridPosition(checkEntity->GetPos());
	int     gridRadius = ceil(checkEntity->GetPerceptionRadius());

	IntVec2 currentPos;

	for ( int y = -gridRadius; y <= gridRadius; ++y )
	{
		for ( int x = -gridRadius; x <= gridRadius; ++x )
		{
			currentPos = {gridPos.x + x, gridPos.y + y};
			currentPos = FitOnGrid(currentPos);

			for ( auto &otherEntity: mGrid[currentPos.y][currentPos.x] )
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

			if ( mGrid[y][x].empty())
			{
				continue;
			}

			DrawRectangleV(start, {size, size}, RED);
		}
	}
}

void Grid::ClearGrid()
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
//	pos.x = std::min(std::max(pos.x, 0), (int) Constants::GridWidth - 1);
//	pos.y = std::min(std::max(pos.y, 0), (int) Constants::GridHeight - 1);

	const int gridWidth  = Constants::GridWidth;
	const int gridHeight = Constants::GridHeight;

	if ( pos.x >= gridWidth )
	{
		pos.x -= gridWidth;
	}
	if ( pos.x < 0 )
	{
		pos.x += gridWidth;
	}

	if ( pos.y >= gridHeight )
	{
		pos.y -= gridHeight;
	}
	if ( pos.y < 0 )
	{
		pos.y += gridHeight;
	}

	return pos;
}