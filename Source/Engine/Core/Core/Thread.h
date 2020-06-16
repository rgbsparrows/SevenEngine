#pragma once

#include <thread>

namespace Thread
{
	bool IsGameThread(std::thread::id _threadId) noexcept;

	void SetGameThreadId(std::thread::id _threadId) noexcept;

	bool IsRenderThread(std::thread::id _threadId) noexcept;

	void SetRenderThread(std::thread::id _threadId) noexcept;
}
