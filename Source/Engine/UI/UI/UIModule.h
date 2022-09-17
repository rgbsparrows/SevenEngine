#pragma once
#include "Core/Modules/ModuleInterface.h"

__interface IUIWindowInterface;

__interface IUIModuleInterface : IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;

	void OnGUI() noexcept;

	void AddWindow(IUIWindowInterface* _window, const std::wstring& _windowTag = std::wstring()) noexcept;
	IUIWindowInterface* GetWindowByTag(const std::wstring& _windowTag) noexcept;
};

IUIModuleInterface* GetUIModule() noexcept;
