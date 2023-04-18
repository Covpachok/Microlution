//
// Created by heylc on 12.04.2023.
//

#include "vegetable.hpp"
#include "logger.hpp"

void Vegetable::Update(float delta)
{
	DEBUG_LOG_INFO("Update");
}

void Vegetable::Draw()
{
	DEBUG_LOG_INFO("Draw");
}

void Vegetable::OnBodyCollisionEnter(Entity &other)
{
	DEBUG_LOG_INFO("Collide");
}
