#include "WindowsPlatformApiImplWarper.h"
#include "InnerWindows.h"

namespace WindowsPlatformApiWarp_Impl
{
	std::wstring ConvertStringToWstring_WindowsImpl(uint32_t _codePage, std::string_view _srcString) noexcept
	{
		std::wstring result;

		size_t wideCharCount = ::MultiByteToWideChar(_codePage, 0, _srcString.data(), static_cast<int>(_srcString.size()), nullptr, 0);

		result.resize(wideCharCount);
		::MultiByteToWideChar(_codePage, 0, _srcString.data(), static_cast<int>(_srcString.size()), &result[0], static_cast<int>(result.size()));

		return result;
	}

	std::string ConvertWstringToString_WindowsImpl(uint32_t _codePage, std::wstring_view _srcString) noexcept
	{
		std::string result;

		size_t mutiCharCount = ::WideCharToMultiByte(_codePage, 0, _srcString.data(), static_cast<int>(_srcString.size()), nullptr, 0, '\0', false);

		result.resize(mutiCharCount);
		::WideCharToMultiByte(_codePage, 0, _srcString.data(), static_cast<int>(_srcString.size()), &result[0], static_cast<int>(result.size()), '\0', false);

		return result;
	}

	uint32_t MessageBox_WindowsImpl(void* _hwnd, std::wstring_view _text, std::wstring_view _caption, uint32_t _type) noexcept
	{
		std::wstring test(_text.begin(), _text.end());
		std::wstring caption(_caption.begin(), _caption.end());
		return MessageBoxW(reinterpret_cast<HWND>(_hwnd), test.c_str(), caption.c_str(), _type);
	}

	std::wstring GetCommandLine_WindowsImpl() noexcept
	{
		return GetCommandLineW();
	}

	std::filesystem::path GetModulePath_WindowsImpl(void* _module, uint32_t* _errorCode) noexcept
	{
		wchar_t moduleFileName[MAX_PATH] = {};
		GetModuleFileNameW(reinterpret_cast<HMODULE>(_module), moduleFileName, MAX_PATH);
		if(_errorCode) *_errorCode = GetLastError();
		return std::filesystem::absolute(moduleFileName);
	}

	void* LoadLibary_WindowsImpl(const std::filesystem::path& _modulePath, uint32_t* _errorCode) noexcept
	{
		void * library = LoadLibraryW(_modulePath.c_str());
		if(_errorCode) *_errorCode = GetLastError();
		return library;
	}

	void FreeLibrary_WindowsImpl(void* _module, uint32_t* _errorCode) noexcept
	{
		FreeLibrary(reinterpret_cast<HMODULE>(_module));
		if(_errorCode) *_errorCode = GetLastError();
	}

	void* LoadProcAddress_WindowsImpl(void* _module, const std::string& _procName, uint32_t* _errorCode) noexcept
	{
		void* address = GetProcAddress(reinterpret_cast<HMODULE>(_module), _procName.c_str());
		if(_errorCode) *_errorCode = GetLastError();
		return address;
	}

	std::wstring FormatMessage_WindowsImpl(uint32_t _errorCode) noexcept
	{
		wchar_t* buffer = nullptr;
		FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			_errorCode,
			0,
			reinterpret_cast<wchar_t*>(&buffer),
			0,
			nullptr);

		std::wstring message = buffer;
		LocalFree(buffer);
		return message;
	}

	void OutputDebugString_WindowsImpl(std::wstring_view _message) noexcept
	{
		std::wstring message(_message.begin(), _message.end());
		OutputDebugStringW(message.c_str());
	}
}
