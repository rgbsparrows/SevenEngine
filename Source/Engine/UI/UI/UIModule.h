#pragma once
#include "Core/Modules/ModuleInterface.h"

__interface IUIWindowInterface;

class IUIModule : public IModuleInterface
{
public:
	template<std::derived_from<IUIWindowInterface> _uiWindowClass>
	IUIWindowInterface* FindOrAddWindow(const std::string& _windowTag = std::string()) noexcept
	{
		using UIWindowClass = _uiWindowClass;
		IUIWindowInterface* uiWindow = GetWindowByTag(_windowTag);
		if (uiWindow == nullptr)
		{
			uiWindow = new UIWindowClass;
			AddWindow(uiWindow, _windowTag);
		}
		return uiWindow;
	}

	virtual bool Init() noexcept = 0;
	virtual void Clear() noexcept = 0;

	virtual void OnGUI() noexcept = 0;

	virtual void AddWindow(IUIWindowInterface* _window, const std::string& _windowTag = std::string()) noexcept = 0;
	virtual IUIWindowInterface* GetWindowByTag(const std::string& _windowTag) noexcept = 0;
};

IUIModule* GetUIModule() noexcept;
