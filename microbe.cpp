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
#include "food.hpp"
#include "stuff/global.hpp"

using namespace std;

const int   kMinSatiety            = 0;
const int   kSatietyDropValue      = 2;
const int   kSatietyDropTime       = 5;
const float kStarvationTimeToDeath = 30.f;
const int   kStartingSatiety       = 10;

const float kReproductionDelay = 15;

/* RANDOMIZATION CONSTANTS */
const int rMinSatiety     = 75;
const int rMaxSatietyRand = 125;

const float rMinMovementSpeed = Constants::CellSize * 2;
const float rMaxMovementSpeed = rMinMovementSpeed * 1.5f;
const float rMinRotationSpeed = 2;
const float rMaxRotationSpeed = 4;

const float rMinPerceptionRadius = 5;
const float rMaxPerceptionRadius = 7.5f;

const float rMinBodyRadius = 0.75f;
const float rMaxBodyRadius = 1;

const float rMinChangeDirectionTime = 5;
const float rMaxChangeDirectionTime = 15;


Microbe::Microbe(Entity::Type type) :
		Entity(Vector2Zero())
{
	mPos.x = static_cast<float>(GetRandomValue(0, Constants::ScreenWidth));
	mPos.y = static_cast<float>(GetRandomValue(0, Constants::ScreenHeight));

	if ( type == eNone )
	{
		mType = static_cast<Type>(GetRandomValue(Entity::ePredator, Entity::eHerbivorous));
		if ( mType == Entity::ePredator )
		{
			mType = static_cast<Type>(GetRandomValue(Entity::ePredator, Entity::eHerbivorous));
		}
	}
	else
	{
		mType = type;
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

	/** FOOD RELATED */
	mSatietyDropTimer.Update(delta);
	if ( mSatietyDropTimer.IsElapsed())
	{
		mSatiety = max(mSatiety - kSatietyDropValue, kMinSatiety);
		mSatietyDropTimer.Reset();
	}

	if ( mSatiety == kMinSatiety )
	{
		mStarvationTime += delta;
	}
	else
	{
		mStarvationTime = 0;
	}

	if ( mStarvationTime >= kStarvationTimeToDeath )
	{
		Kill();
		return;
	}

//	mColor.b       = mOriginalColor.b * ( 1 - mSatiety / mMaxSatiety );
	RecalculateMovementSpeed();

	/** MOVEMENT */
//	mCurrentMovementSpeed *= static_cast<float>(mSatiety) / static_cast<float>(mMaxSatiety);

	Rotate(delta);
	Move(delta);

	/** DIRECTION BY STATE */
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
	// Avoid stacking of several microbes on each other
	if ( other.GetType() == Entity::ePredator || other.GetType() == Entity::eHerbivorous )
	{
		mPos = Vector2Add(mPos, Vector2Scale(Vector2Normalize(Vector2Subtract(mPos, other.GetPos())), 0.5f));
		OnOutOfBounds();
	}

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
				if ( SatedEnough())
				{
					return;
				}
				Eat(*dynamic_cast<Food *>(&other));
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
				if ( mMovementState == MovementState::eFleeing || SatedEnough())
				{
					return;
				}
				Eat(*dynamic_cast<Food *>(&other));
				break;
			default:
				break;
		}
	}
}

void Microbe::OnPerceptionCollisionEnter(Entity &other)
{
#if 0
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
#endif

	if ( ShouldFleeFrom(other.GetType()))
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
	else if ( ShouldChase(other.GetType()))
	{
		Vector2 pos      = other.GetPos();
		float   distance = Vector2Distance(mPos, pos);
		if ( distance <= mChasingTargetDistance )
		{
			mChasingTargetDistance = distance;
			mChasingTargetPos      = pos;
		}
		mMovementState   = MovementState::eChasing;
	}
	else if ( ShouldReproduceWith(other.GetType()) && other.CanReproduce())
	{
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
	string ret = "Microbe - " + Entity::ToString() + " food: " + to_string(mSatiety) + "/" + to_string(mMaxSatiety) +
	             " starvation:" +
	             to_string(mStarvationTime) + "/" + to_string(kStarvationTimeToDeath);

	return ret;
}

void Microbe::Move(float delta)
{
	if ( mShouldMove )
	{
		mVelocity.x = mCurrentMovementSpeed * mLookingDirection.x;
		mVelocity.y = mCurrentMovementSpeed * mLookingDirection.y;
	}

	mAcceleration.x = -mVelocity.x * 0.8f;
	mAcceleration.y = -mVelocity.y * 0.8f;

	mVelocity.x += mAcceleration.x * delta;
	mVelocity.y += mAcceleration.y * delta;
	mPos.x += mVelocity.x * delta;
	mPos.y += mVelocity.y * delta;

	OnOutOfBounds();
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

#if 1
	if ( GlobalDebug::gDrawDebugDirection )
	{
		Vector2 lineEnd = {mPos.x + ( mLookingDirection.x * Constants::CellSize * 2.5f ),
		                   mPos.y + ( mLookingDirection.y * Constants::CellSize * 2.5f )};

		DrawLine((int) mPos.x, (int) mPos.y, (int) lineEnd.x, (int) lineEnd.y, GREEN);

		lineEnd = {mPos.x + ( mTargetDirection.x * Constants::CellSize * 2 ),
		           mPos.y + ( mTargetDirection.y * Constants::CellSize * 2 )};

		DrawLine((int) mPos.x, (int) mPos.y, (int) lineEnd.x, (int) lineEnd.y, BLUE);
	}

	if ( GlobalDebug::gDrawDebugPerceptionRadius )
	{
		DrawCircleSectorLines(mPos, mPerceptionRadius * Constants::CellSize, 0, 360, 18, {255, 0, 0, 128});
	}
#endif
}

void Microbe::Initialize()
{
	mPerceptionRadius = GetRandomFloat(rMinPerceptionRadius, rMaxPerceptionRadius);
	mBodyRadius       = GetRandomFloat(rMinBodyRadius, rMaxBodyRadius);


	/* TIMERS */
	mChangeDirectionTimer.SetDelay(GetRandomFloat(rMinChangeDirectionTime, rMaxChangeDirectionTime));
	mReproductionDelayTimer.SetDelay(kReproductionDelay);
	mSatietyDropTimer.SetDelay(kSatietyDropTime);

	mMaxSatiety = GetRandomValue(rMinSatiety, rMaxSatietyRand);
	mSatiety    = kStartingSatiety;


	/* MOVEMENT */
	mMovementSpeed = GetRandomFloat(rMinMovementSpeed, rMaxMovementSpeed);
	mRotationSpeed = GetRandomFloat(rMinRotationSpeed, rMaxRotationSpeed);

	RecalculateMovementSpeed();

	mAcceleration      = Vector2Zero();
	mRotationAngle     = PI / 4;
	mRotationDirection = 1; // -1 - left / 1 - right

	mShouldMove = true;

	ChangeDirection();


	/* COLOR */
	float     ratioR, ratioG, ratioB;
	const int maxColorValue = 255;

	float minR = mType == ePredator ? maxColorValue / 2 : 0;
	float minG = mType == eHerbivorous ? maxColorValue / 2 : 0;
	float minB = maxColorValue / 4;

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

	mColor         = {r, g, b, 215};
	mOriginalColor = mColor;
}

void Microbe::OnOutOfBounds()
{
	if ( mPos.x > Constants::ScreenWidth )
	{
		mPos.x = 0.5f;
	}
	else if ( mPos.x < 0 )
	{
		mPos.x = Constants::ScreenWidth - 0.5f;
	}

	if ( mPos.y > Constants::ScreenHeight )
	{
		mPos.y = 0.5f;
	}
	else if ( mPos.y < 0 )
	{
		mPos.y = Constants::ScreenHeight - 0.5f;
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

	other.ReduceSatiety(kMicrobeNutritionValue);
	ReduceSatiety(kMicrobeNutritionValue);

	EntityManager::GetInstance().SpawnMicrobe(new Microbe(mPos, mType));
	if ( mType == eHerbivorous )
	{
		EntityManager::GetInstance().SpawnMicrobe(new Microbe(mPos, mType));
	}
}

void Microbe::Eat(Food &food)
{
	mSatiety = min(mSatiety + food.GetNutritionValue(), mMaxSatiety);
	food.Kill();
}

