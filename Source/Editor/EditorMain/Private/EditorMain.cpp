#include "UI/UIModule.h"
#include "WorldContentWindow.h"
#include "Core/Modules/ModuleInterface.h"

class SEditorModuleImpl : public IModuleInterface
{
public:
	bool Init() noexcept override
	{
		SUIWorldContentWindow* worldContentWindow = new SUIWorldContentWindow;
		GetUIModule()->AddWindow(L"", worldContentWindow);
		return true;
	}

	void Clear() noexcept override {}

private:

};

REGIST_MODULE(L"EditorModule", SEditorModuleImpl)