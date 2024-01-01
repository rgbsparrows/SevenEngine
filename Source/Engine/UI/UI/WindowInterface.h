#pragma once

__interface IUIWindowInterface
{
	bool IsWindowOpen() noexcept;

	void OnGui() noexcept;
	void Release() noexcept;
};

class SUIStandardWindow : public IUIWindowInterface
{
public:
	bool IsWindowOpen() noexcept { return mIsWindowOpen; }

	void Release() noexcept {}

protected:
	bool mIsWindowOpen = true;
};

class SUILongLivedWindow : public IUIWindowInterface
{
	bool IsWindowOpen() noexcept { return true; }
	void Release() noexcept {}
};
