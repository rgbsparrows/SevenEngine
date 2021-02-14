#pragma once

#include "RDI/RDIModule.h"

using SRDICreateFactoryFunc = IRDIFactory * () noexcept;

struct SRDIFactoryInfo
{
	std::wstring_view mFactoryName;
	SRDICreateFactoryFunc* mCreateFunc;
	IRDIFactory* mFactory;
	uint32_t mRefCount;
};

class SRDIModuleImpl : public IRDIModuleInterface
{
	bool Init() noexcept override;
	void Clear() noexcept override;

	IRDIFactory* CreateRDIFactory(std::wstring_view _rdiName) noexcept override;
	IRDIFactory* GetRDIFactory(std::wstring_view _rdiName) noexcept override;
	void ReleaseRDIFactory(IRDIFactory* _rdiFactory) noexcept override;

private:
	std::vector<SRDIFactoryInfo> mRDIFactoryInfos;
};

SRDIModuleImpl* GetRDIModuleImpl() noexcept;
