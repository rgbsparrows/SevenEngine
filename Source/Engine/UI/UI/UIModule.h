#pragma once
#include "Core/Modules/ModuleInterface.h"

__interface UWindowInterface;

__interface IUIModuleInterface : IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;

	void OnGUI() noexcept;

	void AddWindow(const std::wstring& _windowTag, UWindowInterface* _window) noexcept;
	UWindowInterface* GetWindowByTag(const std::wstring& _windowTag) noexcept;
};

IUIModuleInterface* GetUIModule() noexcept;
