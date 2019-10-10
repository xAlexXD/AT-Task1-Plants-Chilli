#include "..\includes\Timer.h"

Timer::Timer()
{
	_last = std::chrono::steady_clock::now();
}

float Timer::Mark()
{
	const auto old = _last;
	_last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> framtTime = _last - old;
	return framtTime.count();
}

float Timer::Peek() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - _last).count();
}
