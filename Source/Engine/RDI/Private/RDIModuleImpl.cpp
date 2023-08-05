#include "RDIModuleImpl.h"
#include "Core/Util/Assert.h"
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

REGIST_MODULE("RDIModule", SRDIModuleImpl)


bool SRDIModuleImpl::Init() noexcept
{
	GRDIModuleImpl = this;

	mRdiFactory = new SD3D12Factory;
	mRdiFactory->Create();

	return true;
}

void SRDIModuleImpl::Clear() noexcept
{
	mRdiFactory->Release();
	mRdiFactory = nullptr;

	GRDIModuleImpl = nullptr;
}

TODO("RDI模块中应该还有很多对象的Release操作没有正确处理，然后对象运行的话可以直接跑Imgui的时候测一下，对象是否完全释放之类的操作的话也可以顺便弄了，dx12debug层应该都会有提供")
