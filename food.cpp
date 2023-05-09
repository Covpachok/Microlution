#include "food.hpp"

const float kMeatSpoilageTime        = 15;
const float kVegetableSpoilageTime   = 60;
const Color kVegetableColor          = {0, 255, 0, 100};
const Color kMeatColor               = {255, 0, 0, 100};
const int   kVegetableNutritionValue = 10;


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
	DrawCircleSector(mPos, mBodyRadius * Constants::CellSize, 0, 360, 6, mColor);
}

void Food::OnDeath()
{
	mColor.a = 0;
}

void Food::Initialize()
{
	if(mType == Entity::eVegetable)
	{
		mColor      = kVegetableColor;
		mBodyRadius = 0.5f;
	}
	else
	{
		mColor      = kMeatColor;
		mBodyRadius = static_cast<float>(mNutritionValue) / 50.f;
	}

	mPerceptionRadius = 0;

	mSpoilageTimer.SetDelay(mType == Entity::eVegetable ? kVegetableSpoilageTime : kMeatSpoilageTime);
}
