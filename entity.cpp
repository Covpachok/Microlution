//
// Created by heylc on 16.04.2023.
//

#include "entity.hpp"

std::string TypeString[Entity::eAmount] = {"Predator", "Herbivorous", "Vegetable", "None"};

Entity::~Entity()
{
// delete from grid
}

std::string Entity::ToString() const
{
	using namespace std;

	string ret = TypeString[mType] + " x[" + to_string(mPos.x) + "] y[" + to_string(mPos.y) + "]";

	return ret;
}
