#include "food.hpp"
#include "constants.hpp"

const float kMeatSpoilageTime      = 10;
const float kVegetableSpoilageTime = 30;

const Color kVegetableColor = {0, 255, 0, 100};
const Color kMeatColor      = {255, 0, 0, 100};

const int kVegetableNutritionValue = 5;

const float kVegetableBodyRadius    = 0.5f;
const float kMeatBodyRadiusModifier = 1 / 25.f;

Food::Food() :
		mNutritionValue(kVegetableNutritionValue)
{
	mPos.x = static_cast<float>(GetRandomValue(0, Constants::ScreenWidth));
	mPos.y = static_cast<float>(GetRandomValue(0, Constants::ScreenHeight));

	mType = Entity::eVegetable;

	Initialize();
}

Food::Food(Entity::Type type, Vector2 pos, int nutritionValue) :
		Entity(pos, type), mNutritionValue(nutritionValue)
{
	Initialize();
}

void Food::Update(float delta)
{
	mSpoilageTimer.Update(delta);
	if ( mSpoilageTimer.IsElapsed())
	{
		Kill();
	}
}

void Food::Draw()
{
	DrawCircleSector(mPos, mBodyRadius * Constants::CellSize, 0, 360, 1, mColor);
}

void Food::OnDeath()
{
	mColor.a = 0;
}

void Food::Initialize()
{
	if ( mType == Entity::eVegetable )
	{
		mColor      = kVegetableColor;
		mBodyRadius = kVegetableBodyRadius;
	}
	else
	{
		mColor      = kMeatColor;
		mBodyRadius = static_cast<float>(mNutritionValue) * kMeatBodyRadiusModifier;
	}

	mPerceptionRadius = 0;

	mSpoilageTimer.SetDelay(mType == Entity::eVegetable ? kVegetableSpoilageTime : kMeatSpoilageTime);
}
