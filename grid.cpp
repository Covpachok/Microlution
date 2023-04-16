//
// Created by heylc on 12.04.2023.
//

#include "grid.hpp"
#include "utils.hpp"
#include "entity.hpp"
#include <cassert>

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
	ClearRegisteredEntites();
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

				mGrid[gridY][gridX].push_back(entity.get());
			}
		}

	}
}

//void Grid::CheckCollisions(size_t id)
//{
//	auto entity = mGrid[y][x].at(id);
//}

void Grid::CheckCollisions(Vector2 pos, float radius)
{
	IntVec2 gridPos = ToGridPosition(pos);

	assert(( gridPos.y >= 0 && gridPos.y < Constants::GridHeight ));
	assert(( gridPos.x >= 0 && gridPos.x < Constants::GridWidth ));

	int gridRadius = 0;

	for ( int y = -gridRadius; y <= gridRadius; ++y )
	{
		for ( int x = -gridRadius; x <= gridRadius; ++x )
		{
			int resX = gridPos.x + x;
			int resY = gridPos.y + y;

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

			for(auto &entity: mGrid[resY][resX])
			{
				if(entity == nullptr)
					continue;

				std::cout << "Collided" << "\n";
			}
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

void Grid::ClearRegisteredEntites()
{
	for ( auto &row: mGrid )
	{
		for ( auto &list: row )
		{
			list.clear();
		}
	}
}

IntVec2 Grid::ToGridPosition(Vector2 pos)
{
	return ToGridPosition(pos.x, pos.y);
}

IntVec2 Grid::ToGridPosition(float x, float y)
{
	IntVec2 ret;
	ret.x = floor(x / Constants::CellSize);
	ret.y = floor(y / Constants::CellSize);
	return ret;
}

size_t Grid::ToGridUnits(float n)
{
	return static_cast<size_t>(n / Constants::CellSize);
}
