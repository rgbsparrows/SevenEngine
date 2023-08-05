#include "Core/Misc/Thread.h"
#include "Core/Misc/Localize.h"
#include "Core/Misc/windowsEx.h"

#include <thread>

std::string GetHResultMessage(HRESULT _hresult) noexcept
{
	wchar_t msgBuffer[512];

	FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		_hresult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		msgBuffer,
		512, NULL);

	return Locale::ConvertWstringToString(msgBuffer);
}

void YieldForSingleObject(HANDLE _handle) noexcept
{
	Thread::YieldWhile(
		[&]() {return WaitForSingleObject(_handle, 0) == WAIT_TIMEOUT; }
	);
}
