//
// Created by heylc on 11.04.2023.
//

#ifndef MICROLUTION_ENTITY_HPP
#define MICROLUTION_ENTITY_HPP

#include <cstdint>
#include <raylib.h>
#include <string>
#include "constants.hpp"

class Entity
{
public:
	enum Type
	{
		ePredator, eHerbivorous, eVegetable, eNone, eAmount
	};

	constexpr static float DefaultBodySize = Constants::CellSize;

public:
	Entity(Vector2 pos = {0, 0}, float perceptionRadius = 0) :
			mPos(pos), mPerceptionRadius(perceptionRadius) {}

	virtual ~Entity();

	virtual void Update(float delta) = 0;
	virtual void Draw() = 0;

	virtual void OnBodyCollisionEnter(Entity &other) = 0;
	virtual void OnPerceptionCollisionEnter(Entity &other) = 0;

	const Vector2 &GetPos() const { return mPos; }

	static float GetBodySize() { return DefaultBodySize; }
	static float GetBodyRadius() { return DefaultBodySize / 2.f; }

	Type GetType() const { return mType; }
	float GetPerceptionRadius() const { return mPerceptionRadius; }

	virtual std::string ToString() const;

protected:
	Type mType = eNone;

	Vector2 mPos;

	float mPerceptionRadius;

private:
	// Can be used to delete entity after collision detection process ended, so none of the iterators will be broken
	size_t mGridPosX, mGridPosY;
	size_t mGridId;
};

#endif //MICROLUTION_ENTITY_HPP
