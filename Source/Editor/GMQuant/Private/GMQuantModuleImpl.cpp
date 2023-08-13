#include "UI/UIModule.h"
#include "GMQuantModuleImpl.h"
#include "GMQuantMainWindow.h"
#include "Core/Modules/ModuleManager.h"

#include "Strategy/MA21Strategy.h"

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

	RegisterQuantStrategy([]() {return new SMA21Strategy; }, u8"21日均线策略");
	RegisterQuantStrategy([]() {return new SMA21Strategy; }, u8"MACD策略");
	RegisterQuantStrategy([]() {return new SMA21Strategy; }, u8"KDJ策略");
	RegisterQuantStrategy([]() {return new SMA21Strategy; }, u8"21日均线策略");
	RegisterQuantStrategy([]() {return new SMA21Strategy; }, u8"21日均线策略");
	RegisterQuantStrategy([]() {return new SMA21Strategy; }, u8"21日均线策略");

	return true;
}

void SGMQuantModuleImpl::Clear() noexcept
{
	SModuleManager::Get().UnloadModule("GMQuantCoreModule");
	SModuleManager::Get().UnloadModule("UIModule");

	GGMQuantModuleImpl = nullptr;
}
