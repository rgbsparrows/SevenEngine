#pragma once
#include "Core/Util/TemplateUtil.h"

struct SClock : TAsSingleton<SClock>
{
	float GetAbsoluteTotalTime()const noexcept { return mAbsoluteTotalTime; }
	float GetAbsoluteDeltaTime()const noexcept { return mAbsoluteDeltaTime; }

	float GetTotalTime()const noexcept { return mTotalTime; }
	float GetDeltaTime()const noexcept { return mDeltaTime; }

	float GetGameClockRatio()const noexcept { return mGameClockRatio; }
	void SetGameClockRatio(float _ratio) noexcept { mGameClockRatio = _ratio; }

	void Tick() noexcept;

private:
	float mAbsoluteTotalTime = 0.f;
	float mAbsoluteDeltaTime = 0.f;
	float mTotalTime = 0.f;
	float mDeltaTime = 0.f;

	float mGameClockRatio = 1.f;
};
