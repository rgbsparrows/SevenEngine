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
	void SetCurrentThreadName(std::string_view _threadName) noexcept;

	constexpr size_t GetFixedThreadCount() noexcept { return 2; } // Game and Render
	size_t GetMaxMinorThreadCount() noexcept;
	size_t GetSubRenderThreadCount() noexcept;

	template<typename _funcType>
	void YieldUntil(_funcType _condition) noexcept
	{
		while (_condition() == false)
			std::this_thread::yield();
	}

	template<typename _valueType>
	void YieldUntilValue(std::atomic<_valueType>& _atomicValue, _valueType _value) noexcept
	{
		while (_atomicValue != _value)
			std::this_thread::yield();
	}

	template<typename _funcType>
	void YieldWhile(_funcType _condition) noexcept
	{
		while (_condition())
			std::this_thread::yield();
	}

	template<typename _valueType>
	void YieldWhileValue(std::atomic<_valueType>& _atomicValue, _valueType _value) noexcept
	{
		while (_atomicValue == _value)
			std::this_thread::yield();
	}

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
