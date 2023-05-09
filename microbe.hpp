#ifndef MICROLUTION_MICROBE_HPP
#define MICROLUTION_MICROBE_HPP

#include "entity.hpp"
#include "stuff/timer.hpp"

class Food;

class Microbe : public Entity
{
	enum class MovementState
	{
		eWandering, eFleeing, eChasing
	};

	const int kMicrobeNutritionValue = 25;

public:
	explicit Microbe(Entity::Type type = Entity::eNone);
	Microbe(Vector2 pos, Entity::Type type);

	void ChangeDirection();
	void ChangeDirection(const Vector2 &newDirection);

	void Update(float delta) override;
	void Draw() override;

	void OnBodyCollisionEnter(Entity &other) override;
	void OnPerceptionCollisionEnter(Entity &other) override;
	void OnDeath() override;

	bool CanReproduce() const override { return mReproductionDelayTimer.IsElapsed() && SatedEnough(); };
	void ResetReproductionTimer() { mReproductionDelayTimer.Reset(); }

	int GetNutritionValue() const override { return mSatiety / 2 + kMicrobeNutritionValue; }

	std::string ToString() const override;

private:
	void Initialize();

	void Move(float delta);
	void Rotate(float delta);
	void OnOutOfBounds();

	void Reproduce(Microbe &other);
	void Eat(Food &food);

	void RecalculateMovementSpeed() { mCurrentMovementSpeed = mMovementSpeed * ( 1.25f - SatietyPercentage()); }

	float SatietyPercentage() const { return static_cast<float>(mSatiety) / static_cast<float>(mMaxSatiety); }
	bool SatedEnough() const { return SatietyPercentage() >= 0.75f; }

	bool ShouldFleeFrom(Type other) const { return mType == eHerbivorous && other == ePredator; }
	bool ShouldChase(Type other) const
	{
		return (( mType == eHerbivorous && other == eVegetable ) ||
		        ( mType == ePredator && other == eMeat ) ||
		        ( mType == ePredator && other == eHerbivorous )) &&
		       !SatedEnough() && mMovementState != MovementState::eFleeing;
	}
	bool ShouldReproduceWith(Type other) const
	{
		return mType == other && mMovementState != MovementState::eFleeing && CanReproduce();
	}

	void ReduceSatiety(int value) { mSatiety = std::min(mSatiety - value, 0); }

protected:
	/* ==== MOVEMENT ==== */
	Vector2 mLookingDirection = {1, 0};
	Vector2 mTargetDirection  = {0, 1};
	Vector2 mNewDirection     = {0, 0};

	Vector2 mVelocity             = {0, 0};
	Vector2 mAcceleration         = {0, 0};
	float   mCurrentMovementSpeed = 0;
	float   mMovementSpeed        = 0;

	float mRotationAngle     = 0;
	float mRotationDirection = 0;
	float mRotationSpeed     = 1.5f;
	float mTargetRotation    = 0.f;

	bool mShouldMove = true;
	/* ================== */

	Entity *mPerceptionCollidedEntity = nullptr;
	float   mPerceptionCollidedDistance = 1000;

	MovementState mMovementState = MovementState::eWandering;

	/* ==== STATS ==== */
	Color mColor         = WHITE;
	Color mOriginalColor = WHITE;

	/* FOOD */
	int mSatiety    = 0;
	int mMaxSatiety = 0;

	float mStarvationTime = 0;
	/* =============== */


	/* ==== UTILITY STUFF ==== */
	Timer mChangeDirectionTimer;
	Timer mSatietyDropTimer;
	Timer mReproductionDelayTimer;

	bool mPassBoundsPrevFrame = false;
	/* ======================= */
};


#endif //MICROLUTION_MICROBE_HPP
