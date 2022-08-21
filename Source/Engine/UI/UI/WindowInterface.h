#pragma once

__interface IUIWindowInterface
{
	bool IsWindowOpen() noexcept;

	void OnGui() noexcept;
	void Release() noexcept;
};
