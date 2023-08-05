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

TODO("RDIģ����Ӧ�û��кܶ�����Release����û����ȷ����Ȼ��������еĻ�����ֱ����Imgui��ʱ���һ�£������Ƿ���ȫ�ͷ�֮��Ĳ����Ļ�Ҳ����˳��Ū�ˣ�dx12debug��Ӧ�ö������ṩ")
