#include "microbe.hpp"

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

const float kPredatorReproductionDelay    = 30;
const float kHerbivorousReproductionDelay = 20;

const unsigned char kDefaultMicrobeColorAlpha = 215;

/* RANDOMIZATION CONSTANTS */
const int rMinSatiety = 75;
const int rMaxSatiety = 125;

const float rMinMovementSpeed = Constants::CellSize * 2;
const float rMaxMovementSpeed = rMinMovementSpeed * 1.5f;
const float rMinRotationSpeed = 2;
const float rMaxRotationSpeed = 4;

const float rMinPerceptionRadius = 7.5f;
const float rMaxPerceptionRadius = 12.5f;

const float rMinBodyRadius = 0.5f;
const float rMaxBodyRadius = 1.f;

const float rMinChangeDirectionTime = 5;
const float rMaxChangeDirectionTime = 15;

const Color rLowHerbivorousColor  = {0, 128, 0, kDefaultMicrobeColorAlpha};
const Color rHighHerbivorousColor = {32, 255, 128, kDefaultMicrobeColorAlpha};

const Color rLowPredatorColor  = {128, 0, 0, kDefaultMicrobeColorAlpha};
const Color rHighPredatorColor = {255, 32, 128, kDefaultMicrobeColorAlpha};

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
		mChangeDirectionTimer.SetDelay(GetRandomFloat(rMinChangeDirectionTime, rMaxChangeDirectionTime));
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

	RecalculateMovementSpeed();

	/** MOVEMENT */
	Rotate(delta);
	Move(delta);

	mPerceptionCollidedDistance = 10000;
	mMovementState              = MovementState::eWandering;
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
				if ( CanReproduce() && other.CanReproduce())
				{
					auto partner = dynamic_cast<Microbe *>(&other);
					Reproduce(*partner);
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
				if ( CanReproduce() && other.CanReproduce())
				{
					auto partner = dynamic_cast<Microbe *>(&other);
					Reproduce(*partner);
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
	Vector2 pos      = other.GetPos();
	float   distance = Vector2Distance(mPos, pos);

	if ( ShouldFleeFrom(other.GetType()))
	{
		if ( distance <= mPerceptionCollidedDistance )
		{
			mPerceptionCollidedDistance = distance;
			mPerceptionCollidedEntity   = &other;
		}
		ChangeDirection(Vector2Subtract(mPos, mPerceptionCollidedEntity->GetPos()));
		mMovementState = MovementState::eFleeing;
	}
	else if ( ShouldChase(other.GetType()) || ( ShouldReproduceWith(other.GetType()) && other.CanReproduce()))
	{
		if ( distance <= mPerceptionCollidedDistance )
		{
			mPerceptionCollidedDistance = distance;
			mPerceptionCollidedEntity   = &other;
		}
		ChangeDirection(Vector2Subtract(mPerceptionCollidedEntity->GetPos(), mPos));
		mMovementState = MovementState::eChasing;
	}
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
	mReproductionDelayTimer.SetDelay(
			mType == eHerbivorous ? kHerbivorousReproductionDelay : kPredatorReproductionDelay);
	mSatietyDropTimer.SetDelay(kSatietyDropTime);

	mMaxSatiety = GetRandomValue(rMinSatiety, rMaxSatiety);
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

	uint8_t r = 0, g = 0, b = 0;
	if ( mType == eHerbivorous )
	{
		r = static_cast<uint8_t>(GetRandomValue(rLowHerbivorousColor.r, rHighHerbivorousColor.r));
		g = static_cast<uint8_t>(GetRandomValue(rLowHerbivorousColor.g, rHighHerbivorousColor.g));
		b = static_cast<uint8_t>(GetRandomValue(rLowHerbivorousColor.b, rHighHerbivorousColor.b));
	}
	else if ( mType == ePredator )
	{
		r = static_cast<uint8_t>(GetRandomValue(rLowPredatorColor.r, rHighPredatorColor.r));
		g = static_cast<uint8_t>(GetRandomValue(rLowPredatorColor.g, rHighPredatorColor.g));
		b = static_cast<uint8_t>(GetRandomValue(rLowPredatorColor.b, rHighPredatorColor.b));
	}

	mColor         = {r, g, b, kDefaultMicrobeColorAlpha};
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
}

void Microbe::Reproduce(Microbe &other)
{
	other.ResetReproductionTimer();
	ResetReproductionTimer();

	other.ReduceSatiety(kMicrobeNutritionValue);
	ReduceSatiety(kMicrobeNutritionValue);

	// Inheritable stats
	const float rMovementSpeedDiff = ( rMaxMovementSpeed - rMinMovementSpeed ) / 10.f;
	float       childMovementSpeed = GetRandomFloat(mMovementSpeed, other.mMovementSpeed);
	childMovementSpeed += GetRandomFloat(-rMovementSpeedDiff, rMovementSpeedDiff);

	const float rRotationSpeedDiff = ( rMaxRotationSpeed - rMinRotationSpeed ) / 10.f;
	float       childRotationSpeed = GetRandomFloat(mRotationSpeed, other.mRotationSpeed);
	childRotationSpeed += GetRandomFloat(-rRotationSpeedDiff, rRotationSpeedDiff);

	const float rPerceptionRadiusDiff = ( rMaxPerceptionRadius - rMinPerceptionRadius ) / 10.f;
	float       childPerceptionRadius = GetRandomFloat(mPerceptionRadius, other.mPerceptionRadius);
	childPerceptionRadius += GetRandomFloat(-rPerceptionRadiusDiff, rPerceptionRadiusDiff);

	const int rSatietyDiff    = ( rMaxSatiety - rMinSatiety ) / 10;
	int       childMaxSatiety = GetRandomValue(mMaxSatiety, other.mMaxSatiety);
	childMaxSatiety += GetRandomValue(-rSatietyDiff, rSatietyDiff);

	const float rBodyRadiusDiff = ( rMaxBodyRadius - rMinBodyRadius ) / 10.f;
	float       childBodyRadius = GetRandomFloat(mBodyRadius, other.mBodyRadius);
	childBodyRadius += GetRandomFloat(-rBodyRadiusDiff, rBodyRadiusDiff);

	unsigned char r          = GetRandomValue(mColor.r, other.mColor.r);
	unsigned char g          = GetRandomValue(mColor.g, other.mColor.g);
	unsigned char b          = GetRandomValue(mColor.b, other.mColor.b);
	Color         childColor = {r, g, b, kDefaultMicrobeColorAlpha};

	auto child = new Microbe(mPos, mType);
	child->mMovementSpeed    = childMovementSpeed;
	child->mRotationSpeed    = childRotationSpeed;
	child->mPerceptionRadius = childPerceptionRadius;
	child->mMaxSatiety       = childMaxSatiety;
	child->mBodyRadius       = childBodyRadius;
	child->mColor            = childColor;

	EntityManager::GetInstance().SpawnMicrobe(child);
}

void Microbe::Eat(Food &food)
{
	mSatiety = min(mSatiety + food.GetNutritionValue(), mMaxSatiety);
	food.Kill();
}

