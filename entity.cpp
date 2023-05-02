//
// Created by heylc on 16.04.2023.
//

#include "entity.hpp"
#include "stuff/logger.hpp"

//ePredator, eHerbivorous, eVegetable, eMeat, eNone, eAmount
std::string TypeString[Entity::eAmount] = {"Predator", "Herbivorous", "Vegetable", "Meat", "None"};

Entity::~Entity() {}

std::string Entity::ToString() const
{
	if ( mType == Entity::eAmount )
	{
		DEBUG_LOG_ERROR("Why the fuck is this entity have eAmount type?");
		return "Weird stuff";
	}

	using namespace std;

	string ret = TypeString[mType] + " x[" + to_string(mPos.x) + "] y[" + to_string(mPos.y) + "]";

	return ret;
}
