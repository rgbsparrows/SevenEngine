#include "UI/UIModule.h"
#include "Core/Modules/ModuleManager.h"
#include "Core/Modules/ModuleInterface.h"

TODO("移除这两行")
#include "WorldContentWindow.h"
#include "FbxImporterWindow.h"

class SEditorModuleImpl : public IModuleInterface
{
public:
	bool Init() noexcept override
	{
		SModuleManager::Get().LoadModule("UIModule");
		TODO("增加窗口自动注册机制，有一部分窗口不应当被人工调用启用，而应当自动启用，尤其针对长期驻存窗口");
		TODO("引入逻辑之后，这里不必要的include 可以移除");;
		GetUIModule()->FindOrAddWindow<SUIFbxImporterMenuWindow>("FbxImporterMenu");
		GetUIModule()->FindOrAddWindow<SUIWorldContentWindow>("WorldContent");
		return true;
	}

	void Clear() noexcept override
	{
		SModuleManager::Get().UnloadModule("UIModule");
	}

private:

};

REGIST_MODULE("EditorModule", SEditorModuleImpl)
