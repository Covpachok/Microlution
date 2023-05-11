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

	// Food do nothing when collide with anything
	void OnBodyCollisionEnter(Entity &other) override {}
	// Food have no perception
	void OnPerceptionCollisionEnter(Entity &other) override {}
	void OnDeath() override;

	int GetNutritionValue() const override { return mNutritionValue; }

private:
	// We have several constructors, so we get common functionality and place it in a separate method
	void Initialize();

private:
	int mNutritionValue = 0;

	// Food will be destroyed over time
	Timer mSpoilageTimer;
};


#endif //MICROLUTION_FOOD_HPP
