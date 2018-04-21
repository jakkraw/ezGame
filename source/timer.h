#pragma once
#include <chrono>

struct Timer {
	using Seconds = double;
	using Duration = std::chrono::duration<double>;
	using Clock = std::chrono::system_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	TimePoint last = Clock::now();
	Seconds delta() const { return Duration(Clock::now() - last).count(); }
	void reset() { last = Clock::now(); }
};