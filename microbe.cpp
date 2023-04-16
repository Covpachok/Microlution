//
// Created by heylc on 02.04.2023.
//

#include <iostream>
#include "microbe.hpp"
#include "raymath.h"
#include "logger.hpp"
#include "utils.hpp"

using namespace std;

const float MicrobeSize = Constants::CellSize;

Microbe::Microbe() :
		Entity(Vector2Zero(), MicrobeSize, MicrobeSize * 2), mAcceleration(Vector2Zero()),
		mLookingDirection({1, 0}), mTargetDirection({0, 1}),
		mVelocity({0, 0}), mRotationAngle(PI / 4), mRotationDirection(1), mShouldMove(true), mColor(WHITE)
{
	mPos.x = static_cast<float>(GetRandomValue(0, Constants::ScreenWidth));
	mPos.y = static_cast<float>(GetRandomValue(0, Constants::ScreenHeight));

	mMovementSpeed = static_cast<float>(GetRandomValue(200, 1000)) / 10.f;
	mRotationSpeed = static_cast<float>(GetRandomValue(50, 200)) / 100.f;

	ChangeDirection();

	float ratioR, ratioG, ratioB;
	ratioR = static_cast<float>(GetRandomValue(0, 100)) / 100.f;
	ratioG = static_cast<float>(GetRandomValue(0, 100)) / 100.f;
	ratioB = static_cast<float>(GetRandomValue(0, 100)) / 100.f;

	float globalRatio = ratioR + ratioG + ratioB;
	ratioR /= globalRatio;
	ratioG /= globalRatio;
	ratioB /= globalRatio;

	uint8_t r, g, b;
	r = static_cast<uint8_t>(ratioR * 255.f);
	g = static_cast<uint8_t>(ratioG * 255.f);
	b = static_cast<uint8_t>(ratioB * 255.f);

//	mColor = {static_cast<uint8_t>(GetRandomValue(160, 255)),
//	          static_cast<uint8_t>(GetRandomValue(160, 255)),
//	          static_cast<uint8_t>(GetRandomValue(160, 255)), 215};
	mColor = {r, g, b, 215};
}

void Microbe::ChangeDirection()
{
	float randNumX = static_cast<float>(GetRandomValue(-100, 100)) / 100.f;
	float randNumY = static_cast<float>(GetRandomValue(-100, 100)) / 100.f;

	ChangeDirection(Vector2Normalize({randNumX, randNumY}));
}

void Microbe::ChangeDirection(const Vector2 &newDirection)
{
	mTargetDirection = newDirection;

	mTargetRotation = V2AngleBetween(mLookingDirection, mTargetDirection);

	mRotationDirection = mTargetRotation > 0 ? 1.f : -1.f;
}

void Microbe::Update(float delta)
{
	if ( !V2Equals(mLookingDirection, mTargetDirection))
	{
		float val   = ( 1.f - Vector2Distance(mLookingDirection, mTargetDirection)) * mRotationSpeed;
		float speed = mRotationSpeed - val;
		float angle = mRotationDirection * speed * delta;
		mLookingDirection = Vector2Rotate(mLookingDirection, angle);
		mRotationAngle += angle * ( 180 / PI );
	}

	if ( mShouldMove )
	{
		mVelocity.x = mMovementSpeed * mLookingDirection.x;
		mVelocity.y = mMovementSpeed * mLookingDirection.y;
	}

	mAcceleration.x = -mVelocity.x * 0.8f;
	mAcceleration.y = -mVelocity.y * 0.8f;

	mVelocity.x += mAcceleration.x * delta;
	mVelocity.y += mAcceleration.y * delta;
	mPos.x += mVelocity.x * delta;
	mPos.y += mVelocity.y * delta;

	if ( mPos.x > Constants::ScreenWidth )
	{
		mPos.x = 0;
	}
	else if ( mPos.x < 0 )
	{
		mPos.x = Constants::ScreenWidth;
	}
	if ( mPos.y > Constants::ScreenHeight )
	{
		mPos.y = 0;
	}
	else if ( mPos.y < 0 )
	{
		mPos.y = Constants::ScreenHeight;
	}
}

void Microbe::Draw()
{
	const Texture2D &texture = TextureHandler::GetInstance().GetTexture();

	Rectangle src    = {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
	float     size   = static_cast<float>(texture.width) * ( MicrobeSize / static_cast<float>(texture.width));
	Rectangle dest   = {mPos.x, mPos.y, size, size};
	Vector2   origin = {size / 2.f, size / 2.f};

	DrawTexturePro(texture, src, dest, origin, mRotationAngle, mColor);

	/* DEBUG LOOK DIRECTION */
//	Vector2 lineEnd = {mPos.x + ( mLookingDirection.x * 50 ),
//	                   mPos.y + ( mLookingDirection.y * 50 )};
//
//	DrawLine((int) mPos.x, (int) mPos.y, (int) lineEnd.x, (int) lineEnd.y, RED);
//
//	lineEnd = {mPos.x + ( mTargetDirection.x * MicrobeSize ),
//	           mPos.y + ( mTargetDirection.y * MicrobeSize )};
//
//	DrawLine((int) mPos.x, (int) mPos.y, (int) lineEnd.x, (int) lineEnd.y, MAGENTA);
}

void Microbe::OnCollisionEnter(Entity &other)
{
	DEBUG_LOG_INFO("Collision detected");
}
