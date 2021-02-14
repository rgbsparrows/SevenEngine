#include "RDIModuleImpl.h"
#include "Core/Macros/Assert.h"
#include "D3D/D3D12/D3D12Factory.h"

static SRDIModuleImpl* GRDIModuleImpl = nullptr;

IRDIModuleInterface* GetRDIModule() noexcept
{
	return GRDIModuleImpl;
}

SRDIModuleImpl* GetRDIModuleImpl() noexcept
{
	return GRDIModuleImpl;
}

REGIST_MODULE(L"RDIModule", SRDIModuleImpl)


bool SRDIModuleImpl::Init() noexcept
{
	GRDIModuleImpl = this;

	mRDIFactoryInfos.push_back(SRDIFactoryInfo{
		L"D3D12",
		[]()noexcept->IRDIFactory* {return new SD3D12Factory; },
		nullptr,
		0
		}
	);

	return true;
}

void SRDIModuleImpl::Clear() noexcept
{
	for (auto& rdiFactoryInfo : mRDIFactoryInfos)
		CHECK(rdiFactoryInfo.mRefCount == 0);

	GRDIModuleImpl = nullptr;
}

IRDIFactory* SRDIModuleImpl::CreateRDIFactory(std::wstring_view _rdiName) noexcept
{
	auto it = std::find_if(mRDIFactoryInfos.begin(), mRDIFactoryInfos.end(), [&](const SRDIFactoryInfo& _info) {return _info.mFactoryName == _rdiName; });

	if (it == mRDIFactoryInfos.end())
		return nullptr;

	if (it->mRefCount != 0)
	{
		it->mRefCount++;
	}
	else
	{
		IRDIFactory* factory = it->mCreateFunc();
		if (factory != nullptr)
		{
			bool res = factory->Create();
			if (!res)
			{
				factory->Release();
			}
			else
			{
				it->mFactory = factory;
				it->mRefCount++;
			}
		}
	}

	return it->mFactory;
}

IRDIFactory* SRDIModuleImpl::GetRDIFactory(std::wstring_view _rdiName) noexcept
{
	auto it = std::find_if(mRDIFactoryInfos.begin(), mRDIFactoryInfos.end(), [&](const SRDIFactoryInfo& _info) {return _info.mFactoryName == _rdiName; });

	if (it == mRDIFactoryInfos.end())
		return nullptr;

	return it->mFactory;
}

void SRDIModuleImpl::ReleaseRDIFactory(IRDIFactory* _rdiFactory) noexcept
{
	auto it = std::find_if(mRDIFactoryInfos.begin(), mRDIFactoryInfos.end(), [&](const SRDIFactoryInfo& _info) {return _info.mFactory == _rdiFactory; });

	if (it == mRDIFactoryInfos.end())
		return;

	it->mRefCount--;
	if (it->mRefCount == 0)
	{
		it->mFactory->Release();
		it->mFactory = nullptr;
	}
}
