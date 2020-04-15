#include "WindowsPlatformApi.h"
#include "WindowsPlatformApiImplWarper.h"
#include "Localize.h"

std::wstring SWindowsPlatformApi::ConvertStringToWstring(uint32_t _codePage, std::string_view _srcString) noexcept
{
	return WindowsPlatformApiWarp_Impl::ConvertStringToWstring_WindowsImpl(_codePage, _srcString);
}

std::string SWindowsPlatformApi::ConvertWstringToString(uint32_t _codePage, std::wstring_view _srcString) noexcept
{
	return WindowsPlatformApiWarp_Impl::ConvertWstringToString_WindowsImpl(_codePage, _srcString);
}

EMessboxResult SWindowsPlatformApi::MessageBox(HWND _hwnd, std::wstring_view _text, std::wstring_view _cation, EMessageBoxFlag _type) noexcept
{
	uint32_t result = WindowsPlatformApiWarp_Impl::MessageBox_WindowsImpl(reinterpret_cast<void*>(_hwnd), _text, _cation, static_cast<uint32_t>(_type));
	return static_cast<EMessboxResult>(result);
}

std::wstring SWindowsPlatformApi::GetCommandLine() noexcept
{
	return WindowsPlatformApiWarp_Impl::GetCommandLine_WindowsImpl();
}

std::filesystem::path SWindowsPlatformApi::GetModulePath(HMODULE _module, SErrorCode* _errorCode) noexcept
{
	return WindowsPlatformApiWarp_Impl::GetModulePath_WindowsImpl(reinterpret_cast<void*>(_module), _errorCode ? reinterpret_cast<uint32_t*>(_errorCode->mErrorCode) : nullptr);
}

HMODULE SWindowsPlatformApi::LoadLibary(const std::filesystem::path& _modulePath, SErrorCode* _errorCode) noexcept
{
	void* dll = WindowsPlatformApiWarp_Impl::LoadLibary_WindowsImpl(_modulePath, _errorCode ? reinterpret_cast<uint32_t*>(_errorCode->mErrorCode) : nullptr);
	return reinterpret_cast<HMODULE>(dll);
}

void SWindowsPlatformApi::FreeLibrary(HMODULE _module, SErrorCode* _errorCode) noexcept
{
	WindowsPlatformApiWarp_Impl::FreeLibrary_WindowsImpl(_module, _errorCode ? reinterpret_cast<uint32_t*>(_errorCode->mErrorCode) : nullptr);
}

void* SWindowsPlatformApi::LoadRawProcAddress(HMODULE _module, std::wstring_view _funcSignature, SErrorCode* _errorCode) noexcept
{
	return WindowsPlatformApiWarp_Impl::LoadProcAddress_WindowsImpl(_module, ConvertWstringToString(Locale::ECodePage::ACP, _funcSignature), _errorCode ? reinterpret_cast<uint32_t*>(_errorCode->mErrorCode) : nullptr);
}

void SWindowsPlatformApi::OutputDebugString(std::wstring_view _message) noexcept
{
	WindowsPlatformApiWarp_Impl::OutputDebugString_WindowsImpl(_message);
}
