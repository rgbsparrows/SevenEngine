#pragma once

#include "GMQuant/GMQuantModule.h"
#include "Core/Class/ClassObject.h"

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

	std::vector<SClassIdentifier> GetCreateQuantStrategyInfoList() const;

private:
	std::vector<SCreateQuantStrategyInfo> mCreateQuantStrategyInfoList;
};

SGMQuantModuleImpl* GetGMQuantModuleImpl() noexcept;