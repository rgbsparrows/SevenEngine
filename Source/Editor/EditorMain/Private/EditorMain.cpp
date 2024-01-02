#include "UI/UIModule.h"
#include "Core/Modules/ModuleManager.h"
#include "Core/Modules/ModuleInterface.h"

class SEditorModuleImpl : public IModuleInterface
{
public:
	bool Init() noexcept override
	{
		SModuleManager::Get().LoadModule("UIModule");
		return true;
	}

	void Clear() noexcept override
	{
		SModuleManager::Get().UnloadModule("UIModule");
	}

private:

};

REGIST_MODULE("EditorModule", SEditorModuleImpl)
