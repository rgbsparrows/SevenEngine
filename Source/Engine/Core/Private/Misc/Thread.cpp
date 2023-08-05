#include "Core/Math/Math.h"
#include "Core/Misc/Thread.h"
#include "Core/Misc/Localize.h"

std::thread::id GGameThreadId;
std::thread::id GRenderThreadId;

namespace Thread
{
	bool IsGameThread(std::thread::id _threadId) noexcept
	{
		return GGameThreadId == _threadId;
	}

	void SetGameThreadId(std::thread::id _threadId) noexcept
	{
		GGameThreadId = _threadId;
	}

	bool IsRenderThread(std::thread::id _threadId) noexcept
	{
		return GRenderThreadId == _threadId;
	}

	void SetRenderThread(std::thread::id _threadId) noexcept
	{
		GRenderThreadId = _threadId;
	}

	void SetCurrentThreadName(std::string_view _threadName) noexcept
	{
		::SetThreadDescription(GetCurrentThread(), Locale::ConvertStringToWstring(_threadName).c_str());
	}

	size_t GetMaxMinorThreadCount() noexcept
	{
		return Math::Max<size_t>(1, std::thread::hardware_concurrency() - GetFixedThreadCount());
	}

	size_t GetSubRenderThreadCount() noexcept
	{
		return Math::Clamp<size_t>(GetMaxMinorThreadCount() / 3, 1, 6);
	}
}
