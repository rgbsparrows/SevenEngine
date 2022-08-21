#pragma once
#include "Core/Modules/ModuleInterface.h"

__interface IUIWindowInterface;

__interface IUIModuleInterface : IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;

	void OnGUI() noexcept;

	void AddWindow(const std::wstring& _windowTag, IUIWindowInterface* _window) noexcept;
	IUIWindowInterface* GetWindowByTag(const std::wstring& _windowTag) noexcept;
};

IUIModuleInterface* GetUIModule() noexcept;
