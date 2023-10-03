#include "UI/UIModule.h"
#include "GMQuantModuleImpl.h"
#include "GMQuantMainWindow.h"
#include "Core/Modules/ModuleManager.h"

SGMQuantModuleImpl* GGMQuantModuleImpl = nullptr;

IGMQuantModule* GetGMQuantModule() noexcept
{
	return GGMQuantModuleImpl;
}

SGMQuantModuleImpl* GetGMQuantModuleImpl() noexcept
{
	return GGMQuantModuleImpl;
}

REGIST_MODULE("GMQuantModule", SGMQuantModuleImpl)

bool SGMQuantModuleImpl::Init() noexcept
{
	GGMQuantModuleImpl = this;

	SModuleManager::Get().LoadModule("GMQuantCoreModule");

	GetUIModule()->AddWindow(new SGMQuantMainWindow());

	return true;
}

void SGMQuantModuleImpl::Clear() noexcept
{
	GGMQuantModuleImpl = nullptr;
}
