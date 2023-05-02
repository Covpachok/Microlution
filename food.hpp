//
// Created by heylc on 26.04.2023.
//

#ifndef MICROLUTION_FOOD_HPP
#define MICROLUTION_FOOD_HPP

#include "entity.hpp"
#include "stuff/timer.hpp"

class Food : public Entity
{
public:
	explicit Food(Entity::Type type = Entity::eVegetable);
	Food(Entity::Type type, Vector2 pos);

	void Update(float delta) override;
	void Draw() override;

	void OnBodyCollisionEnter(Entity &other) override;
	void OnPerceptionCollisionEnter(Entity &other) override;
	void OnDeath() override;

private:
	void Initialize();

private:
	Color mColor = {0, 0, 0, 0};

	Timer mSpoilageTimer;
};


#endif //MICROLUTION_FOOD_HPP
