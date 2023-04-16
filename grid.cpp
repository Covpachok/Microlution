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
	for ( auto &entity: entities )
	{
		float         radius      = entity->GetBodyRadius();
		float         diameter    = radius * 2;
		Vector2       position    = entity->GetPos();
		const Vector2 positions[] = {{0,       0},
		                             {radius,  0},
		                             {0,       radius},
		                             {radius,  radius},
		                             {-radius, 0},
		                             {0,       -radius},
		                             {-radius, -radius},
		                             {radius,  -radius},
		                             {-radius, radius}};

		for ( auto pos: positions )
		{
			pos = Vector2Add(position, pos);
			pos.x = floor(pos.x / diameter);
			pos.y = floor(pos.y / diameter);

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

			auto x = static_cast<size_t>(pos.x), y = static_cast<size_t>(pos.y);
			mGrid[y][x].push_back(entity.get());

//			std::cout << x << " " << y << " REGISTERED " << entity.get() << "\n";

//			Vector2 start = {pos.x * diameter, pos.y * diameter};
//			DrawRectangleV(start, {diameter, diameter}, RED);
		}
	}
}
void Grid::CheckCollisions(size_t x, size_t y, size_t id)
{
	assert(!( y >= 0 && y < Constants::GridHeight ));
	assert(!( x >= 0 && x < Constants::GridWidth ));
	auto entity = mGrid[y][x].at(id);
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

			float   diameter = Constants::CellSize * 2;
			Vector2 start    = {x * diameter, y * diameter};
			DrawRectangleV(start, {diameter, diameter}, RED);
		}
	}
}
