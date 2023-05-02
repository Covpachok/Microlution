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
	// Food/Microbe
	// Food of two types - meat and vegetation
	// Microbe of two types - Predator and Herbivorous
	enum Type
	{
		ePredator, eHerbivorous, eVegetable, eMeat, eNone, eAmount
	};

	constexpr static float DefaultBodySize = 1;

public:
	Entity(Vector2 pos = {0, 0}) :
			mPos(pos) {}

	virtual ~Entity() = 0;

	virtual void Update(float delta) = 0;
	virtual void Draw() = 0;

	virtual void OnBodyCollisionEnter(Entity &other) = 0;
	virtual void OnPerceptionCollisionEnter(Entity &other) = 0;
	virtual void OnDeath() = 0;

	void Kill() { mIsDead = true; OnDeath(); }

	const Vector2 &GetPos() const { return mPos; }

	float GetBodyRadius() const { return mBodyRadius; }
	float GetBodySize() const { return mBodyRadius * 2; }
	float GetPerceptionRadius() const { return mPerceptionRadius; }

	Type GetType() const { return mType; }
	bool IsDead() const { return mIsDead; }
	virtual bool CanReproduce() const { return false; };

	virtual std::string ToString() const;

protected:
	Type mType = eNone;

	Vector2 mPos;

	float mPerceptionRadius = 4;
	float mBodyRadius = 0.75f;

private:
	bool  mIsDead = false;
};

#endif //MICROLUTION_ENTITY_HPP
