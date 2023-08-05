#pragma once
#include "Core/Modules/ModuleInterface.h"

__interface IUIWindowInterface;

__interface IUIModuleInterface : IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;

	void OnGUI() noexcept;

	void AddWindow(IUIWindowInterface* _window, const std::string& _windowTag = std::string()) noexcept;
	IUIWindowInterface* GetWindowByTag(const std::string& _windowTag) noexcept;
};

IUIModuleInterface* GetUIModule() noexcept;
