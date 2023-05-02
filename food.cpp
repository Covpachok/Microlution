//
// Created by heylc on 26.04.2023.
//

#include "food.hpp"
#include "stuff/logger.hpp"

const float kMeatSpoilageTime      = 50;
const float kVegetableSpoliageTime = 300;

Food::Food(Entity::Type type)
{
	mPos.x = static_cast<float>(GetRandomValue(0, Constants::ScreenWidth));
	mPos.y = static_cast<float>(GetRandomValue(0, Constants::ScreenHeight));

	mType = type;

	Initialize();
}

Food::Food(Entity::Type type, Vector2 pos)
{
	mPos = pos;

	mType = type;

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
	mColor            = mType == Entity::eVegetable ? GREEN : RED;
	mBodyRadius       = 0.5f;
	mPerceptionRadius = 0;

	mSpoilageTimer.SetDelay(mType == Entity::eVegetable ? kVegetableSpoliageTime : kMeatSpoilageTime);
}
