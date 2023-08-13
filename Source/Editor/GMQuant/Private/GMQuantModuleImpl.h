#pragma once

#include "GMQuant/GMQuantModule.h"

struct SCreateQuantStrategyInfo
{
	SCreateQuantStrategyType mCreateQuantStrategy;
	std::string mStrategyName;
};

class SGMQuantModuleImpl : public IGMQuantModule
{
public:
	bool Init() noexcept override;
	void Clear() noexcept override;

	void RegisterQuantStrategy(SCreateQuantStrategyType&& _createQuantStrategy, const std::string& _strategyName) noexcept override
	{
		mCreateQuantStrategyInfoList.push_back(SCreateQuantStrategyInfo{ std::move(_createQuantStrategy), _strategyName });
	}

	const std::vector<SCreateQuantStrategyInfo>& GetCreateQuantStrategyInfoList() const { return mCreateQuantStrategyInfoList; }

private:
	std::vector<SCreateQuantStrategyInfo> mCreateQuantStrategyInfoList;
};

SGMQuantModuleImpl* GetGMQuantModuleImpl() noexcept;