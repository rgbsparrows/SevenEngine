#pragma once

#include <chrono>
#include <string>

enum class EAdjustMode
{
	ADJUST_NONE,
	ADJUST_FORWARD,
	ADJUST_BACKWARD,
};

struct SBacktestConfig
{
	// ��ʼʱ��
	std::chrono::system_clock::time_point mStartTime = std::chrono::system_clock::time_point::min();
	// ��ֹʱ��
	std::chrono::system_clock::time_point mEndTime = std::chrono::system_clock::time_point::max();

	// ��ʼ�ֽ�
	float mInitialCash = 1000000.0f;
	// �ɽ�����
	float mTransactionRatio = 1.f;
	// Ӷ�����
	float mCommissionRatio = 0.0002f;
	// �������
	float mSlippageRatio = 0.0001f;
	// ��Ȩģʽ
	EAdjustMode mAdjustMode = EAdjustMode::ADJUST_BACKWARD;
};

struct SBar
{
	std::chrono::system_clock::time_point mTime;
	float mOpen = 0.f;
	float mHigh = 0.f;
	float mLow = 0.f;
	float mClose = 0.f;
	float mVolume = 0.f;
	float mAmount = 0.f;
};
