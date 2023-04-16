//
// Created by heylc on 11.04.2023.
//

#ifndef MICROLUTION_ENTITY_HPP
#define MICROLUTION_ENTITY_HPP

#include <cstdint>
#include "raylib.h"

class Entity
{
public:
	Entity(Vector2 pos = {0, 0}, float bodyRadius = 0, float perceptionRadius = 0) :
			mPos(pos), mBodyRadius(bodyRadius), mPerceptionRadius(perceptionRadius) {}

	virtual ~Entity();

	virtual void Update(float delta) = 0;
	virtual void Draw() = 0;

	virtual void OnCollisionEnter(Entity &other) = 0;

	const Vector2 &GetPos() const { return mPos; }
	float GetBodyRadius() const { return mBodyRadius; }
	float GetPerceptionRadius() const { return mPerceptionRadius; }


protected:
	Vector2 mPos;

	float mBodyRadius;
	float mPerceptionRadius;

private:
	size_t mGridPosX, mGridPosY;
	size_t mGridId;
};

#endif //MICROLUTION_ENTITY_HPP
