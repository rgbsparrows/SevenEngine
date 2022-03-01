#include "Clock.h"

#include <chrono>

SClock GClock;

void SClock::Tick() noexcept
{
	static std::chrono::time_point<std::chrono::steady_clock> lastTickTimePoint = std::chrono::steady_clock::now();

	std::chrono::time_point<std::chrono::steady_clock> nowTimePoint = std::chrono::steady_clock::now();

	int64_t deltaMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(nowTimePoint - lastTickTimePoint).count();

	lastTickTimePoint = nowTimePoint;

	mAbsoluteDeltaTime = deltaMicroseconds * 0.000001f;
	mAbsoluteTotalTime += mAbsoluteDeltaTime;

	mDeltaTime = mAbsoluteDeltaTime * mGameClockRatio;
	mTotalTime += mDeltaTime;
}
