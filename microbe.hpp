//
// Created by heylc on 02.04.2023.
//

#ifndef MICROLUTION_MICROBE_HPP
#define MICROLUTION_MICROBE_HPP

#include "raylib.h"

class Microbe
{
public:
	Microbe();

	void ChangeDirection();
	void ChangeDirection(const Vector2 &newDirection);

	void SetShouldMove(bool b) { mShouldMove = b; }

	void Update(float delta);
	void Draw();

	const Vector2 &GetPos() const { return mPos; }

private:
	Vector2 mPos;

	Vector2 mLookingDirection;
	Vector2 mTargetDirection;

	Vector2 mVelocity;
	Vector2 mAcceleration;

	float mMovementSpeed;

	float mRotationAngle;

	float mRotationDirection;

	float mRotationVelocity;
	float mRotationAcceleration;

	float mRotationSpeed = 1.5f;

	bool mShouldMove = true;

	Color mColor;

	float mTargetRotation = 0.f;

	bool mShouldRotate = true;
};


#endif //MICROLUTION_MICROBE_HPP
