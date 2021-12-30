#pragma once

#include "Core/Misc/windowsEx.h"
#include "Core/Util/Assert.h"

#include <thread>

namespace Thread
{
	bool IsGameThread(std::thread::id _threadId) noexcept;

	void SetGameThreadId(std::thread::id _threadId) noexcept;

	bool IsRenderThread(std::thread::id _threadId) noexcept;

	void SetRenderThread(std::thread::id _threadId) noexcept;

	void SetCurrentThreadName(std::wstring_view _threadName) noexcept;

	struct SDummyLock
	{
		void lock() noexcept {}
		void unlock() noexcept {}
	};

	struct SCriticalSection
	{
		SCriticalSection() noexcept
		{
			CHECK(InitializeCriticalSectionAndSpinCount(&mCriticalSection, 0x00000400) == TRUE);
		}

		~SCriticalSection() noexcept
		{
			DeleteCriticalSection(&mCriticalSection);
		}

		void lock() noexcept
		{
			EnterCriticalSection(&mCriticalSection);
		}

		void unlock() noexcept
		{
			LeaveCriticalSection(&mCriticalSection);
		}

		CRITICAL_SECTION mCriticalSection;
	};
}
