//
// Created by heylc on 18.04.2023.
//

#ifndef MICROLUTION_TIMER_HPP
#define MICROLUTION_TIMER_HPP

#include "raylib.h"

class Timer
{
public:
	Timer(float delayInSeconds = 0) :
			mDelay(delayInSeconds) {}

	void Update(float delta)
	{
		mLifeTime += delta;
	};

	bool IsElapsed() const { return mLifeTime >= mDelay; }

	void Reset() { mLifeTime = 0; }

	void SetDelay(float newDelay) { mDelay = newDelay; }

private:
	float mDelay;
	float mLifeTime = 0;
};

#endif //MICROLUTION_TIMER_HPP
