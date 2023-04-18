//
// Created by heylc on 02.04.2023.
//

#ifndef MICROLUTION_MICROBE_HPP
#define MICROLUTION_MICROBE_HPP

#include "entity.hpp"
#include "timer.hpp"

class Microbe : public Entity
{
public:
	Microbe();

	void ChangeDirection();
	void ChangeDirection(const Vector2 &newDirection);

	void Update(float delta) override;
	void Draw() override;

	void OnBodyCollisionEnter(Entity &other) override;
	void OnPerceptionCollisionEnter(Entity &other) override;

	std::string ToString() const override;

private:
	void Move(float delta);
	void Rotate(float delta);

private:
	/* ==== MOVEMENT ==== */
	Vector2 mLookingDirection;
	Vector2 mTargetDirection;

	Vector2 mVelocity;
	Vector2 mAcceleration;
	float   mMovementSpeed;

	float mRotationAngle;
	float mRotationDirection;
	float mRotationSpeed  = 1.5f;
	float mTargetRotation = 0.f;

	bool mShouldMove = true;
	/* ================== */


	/* ==== STATS ==== */
	Color mColor;
	/* =============== */


	/* ==== UTILITY STUFF ==== */
	Timer mChangeDirectionTimer;
	/* ======================= */
};


#endif //MICROLUTION_MICROBE_HPP
