//
// Created by heylc on 02.04.2023.
//

#include "microbe.hpp"

#include "stuff/logger.hpp"
#include "stuff/utils.hpp"
#include "constants.hpp"
#include "stuff/math.hpp"
#include "entity_manager.hpp"
#include "stuff/timer.hpp"

using namespace std;

Microbe::Microbe() :
		Entity(Vector2Zero())
{
	mPos.x = static_cast<float>(GetRandomValue(0, Constants::ScreenWidth));
	mPos.y = static_cast<float>(GetRandomValue(0, Constants::ScreenHeight));

	mType = static_cast<Type>(GetRandomValue(Entity::ePredator, Entity::eHerbivorous));
	if ( mType == Entity::ePredator )
	{
		mType = static_cast<Type>(GetRandomValue(Entity::ePredator, Entity::eHerbivorous));
	}

	Initialize();
}

Microbe::Microbe(Vector2 pos, Entity::Type type) :
		Entity(pos)
{
	mType = type;

	Initialize();
}

void Microbe::ChangeDirection()
{
	float randNumX = static_cast<float>(GetRandomValue(-100, 100)) / 100.f;
	float randNumY = static_cast<float>(GetRandomValue(-100, 100)) / 100.f;

	ChangeDirection({randNumX, randNumY});
}

void Microbe::ChangeDirection(const Vector2 &newDirection)
{
	mTargetDirection = Vector2Normalize(newDirection);

	mTargetRotation = V2AngleBetween(mLookingDirection, mTargetDirection);

	mRotationDirection = mTargetRotation > 0 ? 1.f : -1.f;

	mChangeDirectionTimer.Reset();
}

void Microbe::Update(float delta)
{
	mReproductionDelayTimer.Update(delta);
	mChangeDirectionTimer.Update(delta);
	if ( mChangeDirectionTimer.IsElapsed())
	{
		ChangeDirection();
	}


	Rotate(delta);
	Move(delta);

	if ( mMovementState != MovementState::eWandering )
	{
		if ( mMovementState == MovementState::eChasing )
		{
			ChangeDirection(Vector2Subtract(mChasingTargetPos, mPos));
			mChasingTargetDistance = 10000;
		}
		else
		{
			ChangeDirection(Vector2Subtract(mPos, mFleeingTargetPos));
			mFleeingTargetDistance = 10000;
		}
	}
	mMovementState = MovementState::eWandering;
}

void Microbe::OnBodyCollisionEnter(Entity &other)
{
	if ( mType == Entity::ePredator )
	{
		switch ( other.GetType())
		{
			case ePredator:
				if ( other.CanReproduce())
				{
					Reproduce(*dynamic_cast<Microbe *>(&other));
				}
				break;
			case eHerbivorous:
				other.Kill();
				break;
			case eMeat:
				other.Kill();
				break;
			default:
				break;
		}
	}
	else if ( mType == Entity::eHerbivorous )
	{
		switch ( other.GetType())
		{
			case ePredator:
				Kill();
				break;
			case eHerbivorous:
				if ( other.CanReproduce())
				{
					Reproduce(*dynamic_cast<Microbe *>(&other));
				}
				break;
			case eVegetable:
				other.Kill();
				break;
			default:
				break;
		}
	}
}

void Microbe::OnPerceptionCollisionEnter(Entity &other)
{
	/*
	if ( other.GetType() != mType )
	{
		mPerceiveHostileMicrobe = true;
		Vector2 pos      = other.GetPos();
		float   distance = Vector2Distance(mPos, pos);
		if ( distance <= mNearestHostileDistance )
		{
			mNearestHostileDistance = distance;
			mNearestHostilePos      = pos;
		}
	}
	 */

	if ( other.GetType() != mType )
	{
		if ( other.GetType() == Entity::ePredator )
		{
			Vector2 pos      = other.GetPos();
			float   distance = Vector2Distance(mPos, pos);
			if ( distance <= mFleeingTargetDistance )
			{
				mFleeingTargetDistance = distance;
				mFleeingTargetPos      = pos;
			}
			mMovementState   = MovementState::eFleeing;
		}
		else
		{
			if (( mType == Entity::ePredator && other.GetType() == Entity::eVegetable ) ||
			    ( mType == Entity::eVegetable && other.GetType() == Entity::eMeat ))
			{
				return;
			}

			if ( mMovementState == MovementState::eFleeing )
			{
				return;
			}

			Vector2 pos      = other.GetPos();
			float   distance = Vector2Distance(mPos, pos);
			if ( distance <= mChasingTargetDistance )
			{
				mChasingTargetDistance = distance;
				mChasingTargetPos      = pos;
			}
			mMovementState   = MovementState::eChasing;
		}
	}
	else
	{
		if ( mMovementState == MovementState::eFleeing || ( !other.CanReproduce() && !CanReproduce()))
		{
			return;
		}

		Vector2 pos      = other.GetPos();
		float   distance = Vector2Distance(mPos, pos);
		if ( distance <= mChasingTargetDistance )
		{
			mChasingTargetDistance = distance;
			mChasingTargetPos      = pos;
		}
		mMovementState   = MovementState::eChasing;
	}

#if 0
	if ( other.GetType() == Entity::ePredator && mType == Entity::eHerbivorous )
	{
		ChangeDirection(Vector2Normalize(Vector2Subtract(mPos, other.GetPos())));
	}
	else if ( other.GetType() == Entity::eHerbivorous && mType == Entity::ePredator )
	{
		ChangeDirection(Vector2Normalize(Vector2Subtract(other.GetPos(), mPos)));
	}
#endif
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

	OutOfBoundsCheck();
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

void Microbe::OnDeath()
{
	mColor.a = 20;
}

void Microbe::Draw()
{
	const Texture2D &texture = TextureHandler::GetInstance().GetTexture();

	Rectangle src    = {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
	float     size   = static_cast<float>(texture.width) *
	                   ( GetBodySize() * Constants::CellSize / static_cast<float>(texture.width));
	Rectangle dest   = {mPos.x, mPos.y, size, size};
	Vector2   origin = {size / 2.f, size / 2.f};

	DrawTexturePro(texture, src, dest, origin, mRotationAngle, mColor);

#if 0
	/* DEBUG LOOK DIRECTION */
	Vector2 lineEnd = {mPos.x + ( mLookingDirection.x * Constants::CellSize * 2.5f ),
	                   mPos.y + ( mLookingDirection.y * Constants::CellSize * 2.5f )};

	DrawLine((int) mPos.x, (int) mPos.y, (int) lineEnd.x, (int) lineEnd.y, GREEN);

	lineEnd = {mPos.x + ( mTargetDirection.x * Constants::CellSize * 2 ),
	           mPos.y + ( mTargetDirection.y * Constants::CellSize * 2 )};

	DrawLine((int) mPos.x, (int) mPos.y, (int) lineEnd.x, (int) lineEnd.y, BLUE);
#endif

#if 0
	/* DEBUG PERCEPTION RADIUS */
	DrawCircleSectorLines(mPos, mPerceptionRadius * Constants::CellSize, 0, 360, 18, RED);
#endif
}

void Microbe::Initialize()
{
	mMovementSpeed = ( static_cast<float>(GetRandomValue(Constants::CellSize * 10, Constants::CellSize * 16)) / 10.f );
	mRotationSpeed = static_cast<float>(GetRandomValue(100, 300)) / 100.f;

	mPerceptionRadius = static_cast<float>(GetRandomValue(30, 60)) / 10.f;

	mChangeDirectionTimer.SetDelay(static_cast<float>(GetRandomValue(20, 50)) / 10.f);
	mReproductionDelayTimer.SetDelay(30);

	mAcceleration      = Vector2Zero();
	mRotationAngle     = PI / 4;
	mRotationDirection = 1;
	mShouldMove        = true;

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

void Microbe::OutOfBoundsCheck()
{
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
#if 0
	bool    shouldChangeDirection = false;
	Vector2 newDirection          = {0, 0};

	if ( mPos.x > Constants::ScreenWidth )
	{
		mPos.x         = Constants::ScreenWidth;
		newDirection.x = -1;
		shouldChangeDirection = true;
	}
	else if ( mPos.x < 0 )
	{
		mPos.x         = 0;
		newDirection.x = 1;
		shouldChangeDirection = true;
	}

	if ( mPos.y > Constants::ScreenHeight )
	{
		mPos.y         = Constants::ScreenHeight;
		newDirection.y = -1;
		shouldChangeDirection = true;
	}
	else if ( mPos.y < 0 )
	{
		mPos.y         = 0;
		newDirection.y = 1;
		shouldChangeDirection = true;
	}

	if ( shouldChangeDirection )
	{
		ChangeDirection(newDirection);
		mChangeDirectionTimer.Reset();
	}
#endif

}

void Microbe::Reproduce(Microbe &other)
{
	other.ResetReproductionTimer();
	ResetReproductionTimer();

	EntityManager::GetInstance().SpawnMicrobe(new Microbe(mPos, mType));
}

