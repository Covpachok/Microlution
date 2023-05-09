#ifndef MICROLUTION_FOOD_HPP
#define MICROLUTION_FOOD_HPP

#include "entity.hpp"
#include "stuff/timer.hpp"

class Food : public Entity
{
public:
	Food();
	Food(Entity::Type type, Vector2 pos, int nutritionValue);

	void Update(float delta) override;
	void Draw() override;

	void OnBodyCollisionEnter(Entity &other) override {}
	void OnPerceptionCollisionEnter(Entity &other) override {}
	void OnDeath() override;

	int GetNutritionValue() const override { return mNutritionValue; }

private:
	void Initialize();

private:
	Color mColor = {0, 0, 0, 0};

	int mNutritionValue;

	Timer mSpoilageTimer;
};


#endif //MICROLUTION_FOOD_HPP
