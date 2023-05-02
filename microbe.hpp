//
// Created by heylc on 02.04.2023.
//

#ifndef MICROLUTION_MICROBE_HPP
#define MICROLUTION_MICROBE_HPP

#include "entity.hpp"
#include "stuff/timer.hpp"

class Microbe : public Entity
{
	enum class MovementState
	{
		eWandering, eFleeing, eChasing
	};

public:
	Microbe();
	Microbe(Vector2 pos, Entity::Type type);

	void ChangeDirection();
	void ChangeDirection(const Vector2 &newDirection);

	void Update(float delta) override;
	void Draw() override;

	void OnBodyCollisionEnter(Entity &other) override;
	void OnPerceptionCollisionEnter(Entity &other) override;
	void OnDeath() override;

	bool CanReproduce() const override { return mReproductionDelayTimer.IsElapsed(); };
	void ResetReproductionTimer() { mReproductionDelayTimer.Reset(); }

	std::string ToString() const override;

private:
	void Initialize();

	void Move(float delta);
	void Rotate(float delta);
	void OutOfBoundsCheck();

	void Reproduce(Microbe &other);

protected:
	/* ==== MOVEMENT ==== */
	Vector2 mLookingDirection = {1, 0};
	Vector2 mTargetDirection  = {0, 1};

	Vector2 mVelocity      = {0, 0};
	Vector2 mAcceleration  = {0, 0};
	float   mMovementSpeed = 0;

	float mRotationAngle     = 0;
	float mRotationDirection = 0;
	float mRotationSpeed     = 1.5f;
	float mTargetRotation    = 0.f;

	bool mShouldMove = true;
	/* ================== */

	float   mFleeingTargetDistance = 1000;
	Vector2 mFleeingTargetPos      = {0, 0};

	float   mChasingTargetDistance = 1000;
	Vector2 mChasingTargetPos      = {0, 0};

	MovementState mMovementState = MovementState::eWandering;

	/* ==== STATS ==== */
	Color mColor = WHITE;

	int mSatiety;
	int mMaxSatiety;
	/* =============== */


	/* ==== UTILITY STUFF ==== */
	Timer mChangeDirectionTimer;
	Timer mSatietyDropTimer;
	Timer mReproductionDelayTimer;
	/* ======================= */
};


#endif //MICROLUTION_MICROBE_HPP
