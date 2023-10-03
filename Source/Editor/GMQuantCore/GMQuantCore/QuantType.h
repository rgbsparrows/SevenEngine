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

enum class EFrequency
{
	FREQUENCY_1MIN,
	FREQUENCY_5MIN,
	FREQUENCY_15MIN,
	FREQUENCY_30MIN,
	FREQUENCY_60MIN,
	FREQUENCY_1DAY,
};

struct SQuote
{
	double mPrice = 0.f; // �۸�
	uint64_t mVolume = 0; // ����
};

struct STick
{
	std::chrono::system_clock::time_point mTime; // ��ǰʱ��
	double mPrice = 0.f; // ��ǰ�۸�
	double mOpen = 0.f; // ���̼�
	double mHigh = 0.f; // ��߼�
	double mLow = 0.f; // ��ͼ�

	uint64_t mCumVolume = 0; // �ܳɽ���
	double mCumAmount; // �ܳɽ���
	double mVolume = 0.f; // ��ǰ�ɽ���
	double mAmount = 0.f; // ��ǰ�ɽ���

	std::vector<SQuote> mBidQuoteList; // ����
	std::vector<SQuote> mAskQuoteList; // ����
};

struct SBar
{
	std::chrono::system_clock::time_point mBob; // ��ʼʱ��
	std::chrono::system_clock::time_point mEob; // ����ʱ��

	double mOpen = 0.f; // ���̼�
	double mHigh = 0.f; // ��߼�
	double mLow = 0.f; // ��ͼ�
	double mClose = 0.f; // ���̼�

	uint64_t mVolume = 0; // �ɽ���
	double mAmount = 0.f; // �ɽ���
};

enum class EBoard
{
	BOARD_A, // A������
	BOARD_B, // B������
	BOARD_CY, // ��ҵ��
	BOARD_KC, // �ƴ���
	BOARD_ETF, // ETF
};

struct SStockInfo
{
	EBoard mBoard; // ���
	std::string mSymbolName; // ��Ʊ����
	std::string mAbbr; // ��Ʊ���
	double mPriceTick; // �۸���С�䶯��λ
	std::chrono::system_clock::time_point mListDate; // ��������
	std::chrono::system_clock::time_point mDelistDate; // ��������

	bool mIsSuspended; // �Ƿ�ͣ��
	bool mIsST; // �Ƿ�ST
	float mAdjFactor; // ��Ȩ����
};

struct SCash
{
	double mBalance = 0.f; // ���
	double mMarketValue = 0.f; // �ֲ���ֵ
};

struct SPosition
{
	std::string mSymbol; // ��Ʊ����
	double mMarketValue; // �ֲ���ֵ

	uint64_t mVolumeToday; // ���ճֲ���
	uint64_t mVolume; // �ֲ���
	
	double mPrice; // �ּ�
	double mCost; // �ɱ�
	
	std::chrono::system_clock::time_point mCreateTime; // ����ʱ��
	std::chrono::system_clock::time_point mUpdateTime; // ����ʱ��
};
