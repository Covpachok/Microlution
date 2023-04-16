//
// Created by heylc on 12.04.2023.
//

#ifndef MICROLUTION_VEGETABLE_HPP
#define MICROLUTION_VEGETABLE_HPP

#include "entity.hpp"

class Vegetable : public Entity
{
public:
	Vegetable() = default;

	void Update(float delta) override;
	void Draw() override;

	void OnCollisionEnter(Entity &other) override;

private:
};


#endif //MICROLUTION_VEGETABLE_HPP
