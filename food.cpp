//
// Created by heylc on 26.04.2023.
//

#include "food.hpp"
#include "stuff/logger.hpp"

const float      kMeatSpoilageTime        = 15;
const float      kVegetableSpoilageTime   = 60;
const Color      kVegetableColor          = {0, 255, 0, 100};
const Color      kMeatColor               = {255, 0, 0, 100};
static const int kVegetableNutritionValue = 10;


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
	DrawCircleSector(mPos, mBodyRadius * Constants::CellSize, 0, 360, 8, mColor);
}

void Food::OnBodyCollisionEnter(Entity &other)
{

}

void Food::OnPerceptionCollisionEnter(Entity &other)
{

}

void Food::OnDeath()
{
	mColor.a = 0;
}

void Food::Initialize()
{
	mColor            = mType == Entity::eVegetable ? kVegetableColor : kMeatColor;
	mBodyRadius       = 0.5f;
	mPerceptionRadius = 0;

	mSpoilageTimer.SetDelay(mType == Entity::eVegetable ? kVegetableSpoilageTime : kMeatSpoilageTime);
}
