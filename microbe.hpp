//
// Created by heylc on 02.04.2023.
//

#ifndef MICROLUTION_MICROBE_HPP
#define MICROLUTION_MICROBE_HPP

#include "entity.hpp"

class Microbe : public Entity
{
public:
	Microbe();

	void ChangeDirection();
	void ChangeDirection(const Vector2 &newDirection);

	void Update(float delta) override;
	void Draw() override;

	void OnCollisionEnter(Entity &other) override;

private:
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

	Color mColor;
};


#endif //MICROLUTION_MICROBE_HPP
