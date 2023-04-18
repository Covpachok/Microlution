//
// Created by heylc on 02.04.2023.
//

#include "microbe.hpp"

#include "logger.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "math.hpp"
#include "timer.hpp"

using namespace std;

const float gMicrobePerceptionRadius = Entity::GetBodySize() * 3.f;

Microbe::Microbe() :
		Entity(Vector2Zero(), gMicrobePerceptionRadius), mAcceleration(Vector2Zero()),
		mLookingDirection({1, 0}), mTargetDirection({0, 1}),
		mVelocity({0, 0}), mRotationAngle(PI / 4), mRotationDirection(1), mShouldMove(true), mColor(WHITE)
{
	mType = static_cast<Type>(GetRandomValue(Entity::ePredator, Entity::eHerbivorous));

	mPos.x = static_cast<float>(GetRandomValue(0, Constants::ScreenWidth));
	mPos.y = static_cast<float>(GetRandomValue(0, Constants::ScreenHeight));

	mMovementSpeed = ( static_cast<float>(GetRandomValue(Constants::CellSize * 10, Constants::CellSize * 30)) / 10.f );
	mRotationSpeed = static_cast<float>(GetRandomValue(50, 300)) / 100.f;

	mChangeDirectionTimer.SetDelay(static_cast<float>(GetRandomValue(20, 50)) / 10.f);
	ChangeDirection();



#if 1
	/* Balanced color value calculation */


	float     ratioR, ratioG, ratioB;
	const int maxColorValue = 255;

	float minR = mType == ePredator ? maxColorValue / 2 : 0;
	float minG = mType == eHerbivorous ? maxColorValue / 2 : 0;
	float minB = 0;

	float maxR = mType == ePredator ? maxColorValue : 0;
	float maxG = mType == eHerbivorous ? maxColorValue : 0;
	float maxB = maxColorValue / 2;

	ratioR = static_cast<float>(GetRandomValue(minR, maxR));
	ratioG = static_cast<float>(GetRandomValue(minG, maxG));
	ratioB = static_cast<float>(GetRandomValue(minB, maxB));

	const int chaosMinMax = 100;
	auto      chaos       = static_cast<float>(GetRandomValue(-chaosMinMax, chaosMinMax));

	float globalRatio = ratioR + ratioG + ratioB + chaos;
	ratioR /= globalRatio;
	ratioG /= globalRatio;
	ratioB /= globalRatio;

	uint8_t r, g, b;
	r = static_cast<uint8_t>(ratioR * 255);
	g = static_cast<uint8_t>(ratioG * 255);
	b = static_cast<uint8_t>(ratioB * 255);

	mColor = {r, g, b, 215};
#endif

#if 0
	/* Dumb color value calculation */

	uint8_t   r, g, b;
	const int maxColorValue = 255;
	r      = GetRandomValue(0, maxColorValue);
	g      = GetRandomValue(0, maxColorValue);
	b      = GetRandomValue(0, maxColorValue);
	mColor = {r, g, b, 215};
#endif
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

	mChangeDirectionTimer.Reset();
}

void Microbe::Update(float delta)
{
	mChangeDirectionTimer.Update(delta);
	if ( mChangeDirectionTimer.IsElapsed())
	{
		ChangeDirection();
		DEBUG_LOG_INFO("Changing direction");
	}

	Rotate(delta);
	Move(delta);
}

void Microbe::Draw()
{
	const Texture2D &texture = TextureHandler::GetInstance().GetTexture();

	Rectangle src    = {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
	float     size   = static_cast<float>(texture.width) * ( GetBodySize() / static_cast<float>(texture.width));
	Rectangle dest   = {mPos.x, mPos.y, size, size};
	Vector2   origin = {size / 2.f, size / 2.f};

	DrawTexturePro(texture, src, dest, origin, mRotationAngle, mColor);

#if 1
	/* DEBUG LOOK DIRECTION */
	Vector2 lineEnd = {mPos.x + ( mLookingDirection.x * GetBodyRadius() * 2 ),
	                   mPos.y + ( mLookingDirection.y * GetBodyRadius() * 2 )};

	DrawLine((int) mPos.x, (int) mPos.y, (int) lineEnd.x, (int) lineEnd.y, GREEN);

	lineEnd = {mPos.x + ( mTargetDirection.x * GetBodyRadius()),
	           mPos.y + ( mTargetDirection.y * GetBodyRadius())};

	DrawLine((int) mPos.x, (int) mPos.y, (int) lineEnd.x, (int) lineEnd.y, BLUE);
#endif

#if 0
	/* DEBUG PERCEPTION RADIUS */
	DrawCircleSectorLines(mPos, mPerceptionRadius, 0, 360, 18, RED);
#endif
}

void Microbe::OnBodyCollisionEnter(Entity &other)
{
//	DEBUG_LOG_INFO("Body collides");
}

void Microbe::OnPerceptionCollisionEnter(Entity &other)
{
//	DEBUG_LOG_INFO("Percepts something");
	if ( other.GetType() == Entity::ePredator && mType == Entity::eHerbivorous )
	{
		ChangeDirection(Vector2Normalize(Vector2Subtract(mPos, other.GetPos())));
	}
	if ( other.GetType() == Entity::eHerbivorous && mType == Entity::ePredator )
	{
		ChangeDirection(Vector2Normalize(Vector2Subtract(other.GetPos(), mPos)));
	}
}

std::string Microbe::ToString() const
{
	string ret = "Microbe - " + Entity::ToString();

	return ret;
}

void Microbe::Move(float delta)
{
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

void Microbe::Rotate(float delta)
{
	if ( !V2Equals(mLookingDirection, mTargetDirection))
	{
		float val   = ( 1.f - Vector2Distance(mLookingDirection, mTargetDirection)) * mRotationSpeed;
		float speed = mRotationSpeed - val;
		float angle = mRotationDirection * speed * delta;
		mLookingDirection = Vector2Rotate(mLookingDirection, angle);
		mRotationAngle += angle * ( 180 / PI );
	}
}
