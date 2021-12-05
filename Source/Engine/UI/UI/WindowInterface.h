#pragma once

__interface UWindowInterface
{
	bool IsWindowOpen() noexcept;

	void OnGui() noexcept;
	void OnClose() noexcept;
	void Release() noexcept;
};
