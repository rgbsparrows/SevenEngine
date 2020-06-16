#pragma once

#include "UI/UIModule.h"
#include "Core/Allocator/RawAllocator.h"
#include "WindowsPlatform/WindowsTypes.h"
#include "UI/UIInterface/UIWinMessageHandlerInterface.h"

class SUIModuleImpl : public IUIModuleInterface
{
public:
	bool Init() noexcept override;
	void Clear() noexcept override;

	void ProcessMessage() noexcept override;

	SUIObject* CreateUIObject(uint64_t _typeHash) noexcept override;

	SUIObject* CreateUIObject(const IClassObjectInterface* _classObject) noexcept override;

	SUIObject* CreateUIObject(const IUIClassObjectInterface* _classObject) noexcept override;

	void RegistWindow(HWND _hwnd, IUIWinMessageHandler* _handler) noexcept;

	void UnregistWindow(HWND _hwnd) noexcept;

	IUIWinMessageHandler* FindHandler(HWND _hwnd) noexcept;

	std::wstring_view GetDefaultWindowClassName() noexcept { return L"SE Window"; }

private:
	SRawAllocator mAllocator;

	std::map<HWND, IUIWinMessageHandler*> mHwndHandlerMap;
};

SUIModuleImpl* GetUIModuleImpl() noexcept;
