#include "UIModuleImpl.h"
#include "Logging/Logging.h"
#include "WindowsConstants.h"
#include "WindowsPlatformApi.h"
#include "Modules/ModuleInterface.h"

SUIModuleImpl* GUIModuleImpl = nullptr;

IUIModuleInterface* GetUIModule() noexcept
{
	return GUIModuleImpl;
}

SUIModuleImpl* GetUIModuleImpl() noexcept
{
	return GUIModuleImpl;
}


intptr_t _stdcall GWNDPROC(HWND _hwnd, EWinMessage _message, uintptr_t _wparam, intptr_t _lparam)
{
	IUIWinMessageHandler* handler = GetUIModuleImpl()->FindHandler(_hwnd);
	if (handler != nullptr && handler->ProcessMessage(_message, _wparam, _lparam))
		return 0;
	else return SWindowsPlatformApi::DefWindowProc(_hwnd, _message, _wparam, _lparam);
}

bool SUIModuleImpl::Init() noexcept
{
	GUIModuleImpl = this;

	SErrorCode errorCode;
	SWindowsPlatformApi::RegisterClass(EWinClassStyle::CS_DBLCLKS, &GWNDPROC, nullptr, nullptr, nullptr, nullptr, L"", GetDefaultWindowClassName(), nullptr, &errorCode);
	
	if (!errorCode.IsSuccess())
		return false;
	
	return true;
}

void SUIModuleImpl::Clear() noexcept
{
	SWindowsPlatformApi::UnregisterClass(GetDefaultWindowClassName(), nullptr, nullptr);

	GUIModuleImpl = nullptr;
}

void SUIModuleImpl::ProcessMessage() noexcept
{
	SWindowsPlatformApi::ProcessWinMessage();
}

SUIObject* SUIModuleImpl::CreateUIObject(uint64_t _typeHash) noexcept
{
	auto classObject = SClassManager::Get().GetClassObject(_typeHash);
	if (classObject) return CreateUIObject(classObject);
	else return nullptr;
}

SUIObject* SUIModuleImpl::CreateUIObject(const IClassObjectInterface* _classObject) noexcept
{
	if (_classObject->IsDrivedFromAncestor(SUIObject::ClassObjectType::StaticGetClassHash()))
		return CreateUIObject(static_cast<const IUIClassObjectInterface*>(_classObject));
	else return nullptr;
}

SUIObject* SUIModuleImpl::CreateUIObject(const IUIClassObjectInterface* _classObject) noexcept
{
	void* objectBuffer = mAllocator.Allocate(_classObject->GetClassSize(), _classObject->GetClassAlign());
	return _classObject->ConstructObject(objectBuffer);
}

void SUIModuleImpl::RegistWindow(HWND _hwnd, IUIWinMessageHandler* _handler) noexcept
{
#if WITH_DEBUG_CODE
	if (_hwnd == nullptr || _handler == nullptr)
	{
		LogErr(L"错误的输入参数");
		return;
	}

	auto it = mHwndHandlerMap.find(_hwnd);
	if (it == mHwndHandlerMap.end())
	{
		LogErr(L"HWND被重复注册");
		return;
	}

	mHwndHandlerMap.insert(std::make_pair(_hwnd, _handler));
#else
	mHwndHandlerMap[_hwnd] = _handler;
#endif
}

void SUIModuleImpl::UnregistWindow(HWND _hwnd) noexcept
{
#if WITH_DEBUG_CODE
	auto it = mHwndHandlerMap.find(_hwnd);
	if (it != mHwndHandlerMap.end())
		LogErr(L"HWND被重复释放");
	else mHwndHandlerMap.erase(it);
#else
	mHwndHandlerMap.erase(_hwnd);
#endif
}

IUIWinMessageHandler* SUIModuleImpl::FindHandler(HWND _hwnd) noexcept
{
	auto it = mHwndHandlerMap.find(_hwnd);
	if (it != mHwndHandlerMap.end())
		return it->second;
	else return nullptr;
}

REGIST_MODULE(L"UIModule", SUIModuleImpl)
