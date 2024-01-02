#pragma once
#include "Core/Modules/ModuleInterface.h"

#include <functional>

__interface IUIWindowInterface;

class IUIModule : public IModuleInterface
{
public:
	// WindowTag为空是为匿名窗口
	template<std::derived_from<IUIWindowInterface> _uiWindowClass, typename... _argts>
	IUIWindowInterface* FindOrAddWindow(const std::string& _windowTag, _argts&&... _argvs) noexcept
	{
		using UIWindowClass = _uiWindowClass;
		IUIWindowInterface* uiWindow = GetWindowByTag(_windowTag);
		if (uiWindow == nullptr)
		{
			uiWindow = new UIWindowClass(std::forward<_argts>(_argvs)...);
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

void RegisterDefaultWindowFunc_Internal(std::function<void()> _addWindowFunc) noexcept;

#define DECLEAR_DEFAULT_WINDOW(_windowClass, _windowIdentify, _windowName, ...)	struct _windowClass##_##_windowIdentify##_##Register { _windowClass##_##_windowIdentify##_##Register() noexcept { RegisterDefaultWindowFunc_Internal([](){ GetUIModule()->FindOrAddWindow<_windowClass>(_windowName, __VA_ARGS__); }); } } _windowClass##_##_windowIdentify##_##RegisterInstance;
