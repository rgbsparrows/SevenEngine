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

	bool RegisterClass_WindowsImpl(
		uint32_t _style,
		void* _wndProc,
		void* _instance,
		void* _icon,
		void* _cursor,
		void* _background,
		std::wstring_view _className,
		void* _smallIcon,
		uint32_t* _errorCode
	)
	{
		std::wstring className(_className.begin(), _className.end());

		WNDCLASSEXW wndClass;
		wndClass.cbSize = sizeof(WNDCLASSEXW);
		wndClass.style = _style;
		wndClass.lpfnWndProc = reinterpret_cast<LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM)>(_wndProc);
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = DLGWINDOWEXTRA;
		wndClass.hInstance = reinterpret_cast<HINSTANCE>(_instance);
		wndClass.hIcon = reinterpret_cast<HICON>(_icon);
		wndClass.hCursor = reinterpret_cast<HCURSOR>(_cursor);
		wndClass.hbrBackground = reinterpret_cast<HBRUSH>(_background);
		wndClass.lpszClassName = className.c_str();
		wndClass.hIconSm = reinterpret_cast<HICON>(_smallIcon);

		ATOM res = RegisterClassExW(&wndClass);
		if (_errorCode) *_errorCode = GetLastError();
		return res != 0;
	}

	bool GetClassInfo_WindowsImpl(
		void* _instance,
		std::wstring_view _className,
		uint32_t* _style,
		void** _wndProc,
		void** _icon,
		void** _cursor,
		void** _background,
		void** _smallIcon,
		uint32_t* _errorCode
	)
	{
		WNDCLASSEXW wndClass;

		std::wstring className(_className.begin(), _className.end());
		BOOL res = GetClassInfoExW(reinterpret_cast<HINSTANCE>(_instance), className.c_str(), &wndClass);
		if (_errorCode) *_errorCode = GetLastError();

		if (res == TRUE)
		{
			if (_style) *_style = wndClass.style;
			if (_wndProc) *_wndProc = wndClass.lpfnWndProc;
			if (_icon) *_icon = wndClass.hIcon;
			if (_cursor) *_cursor = wndClass.hCursor;
			if (_background) *_background = wndClass.hbrBackground;
			if (_smallIcon) *_smallIcon = wndClass.hIconSm;
		}

		return res == TRUE;
	}

	bool UnregisterClass_WindowsImpl(std::wstring_view _className, void* _instance, uint32_t* _errorCode)
	{
		std::wstring className(_className.begin(), _className.end());
		BOOL res = UnregisterClassW(className.c_str(), reinterpret_cast<HINSTANCE>(_instance));
		if (_errorCode) *_errorCode = GetLastError();
		return res == TRUE;
	}

	void* CreateWindow_WindowsImpl(
		std::wstring_view _className,
		std::wstring_view _windowName,
		uint64_t _style,
		int32_t _x,
		int32_t _y,
		int32_t _width,
		int32_t _height,
		void* _wndParent,
		void* _menu,
		void* _instance,
		void* _param,
		uint32_t* _errorCode
	)
	{
		std::wstring className(_className.begin(), _className.end());
		std::wstring windowName(_windowName.begin(), _windowName.end());

		uint32_t exStyle = static_cast<uint32_t>(_style >> 32U);
		uint32_t style = static_cast<uint32_t>(_style & 0x00000000ffffffffULL);
		
		HWND hwnd = CreateWindowExW(exStyle, className.c_str(), windowName.c_str(), style, _x, _y, _width, _height, reinterpret_cast<HWND>(_wndParent), reinterpret_cast<HMENU>(_menu), reinterpret_cast<HINSTANCE>(_instance), _param);
		if (_errorCode) *_errorCode = GetLastError();
		return hwnd;
	}

	void* FindWindow_WindowsImpl(void* _wndParent, void* _wndChildAfter, std::wstring_view _className, std::wstring_view _windowName, uint32_t* _errorCode)
	{
		std::wstring className(_className.begin(), _className.end());
		std::wstring windowName(_windowName.begin(), _windowName.end());

		HWND hwnd = FindWindowExW(reinterpret_cast<HWND>(_wndParent), reinterpret_cast<HWND>(_wndChildAfter), className.c_str(), windowName.c_str());
		if (_errorCode) *_errorCode = GetLastError();
		return hwnd;
	}

	bool DestroyWindow_WindowsImpl(void* _wnd, uint32_t* _errorCode)
	{
		BOOL res = DestroyWindow(reinterpret_cast<HWND>(_wnd));
		if (_errorCode) *_errorCode = GetLastError();
		return res == TRUE;
	}
}
