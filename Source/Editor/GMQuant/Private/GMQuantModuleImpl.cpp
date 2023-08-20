#include "UI/UIModule.h"
#include "GMQuantModuleImpl.h"
#include "GMQuantMainWindow.h"
#include "Core/Modules/ModuleManager.h"

#include "Strategy/MAStrategy.h"

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
	SModuleManager::Get().LoadModule("UIModule");

	GetUIModule()->FindOrAddWindow<SGMQuantMainWindow>();

	RegisterQuantStrategy([]() {return new SMAStrategy; }, u8"���߲���");

	return true;
}

void SGMQuantModuleImpl::Clear() noexcept
{
	SModuleManager::Get().UnloadModule("GMQuantCoreModule");
	SModuleManager::Get().UnloadModule("UIModule");

	GGMQuantModuleImpl = nullptr;
}