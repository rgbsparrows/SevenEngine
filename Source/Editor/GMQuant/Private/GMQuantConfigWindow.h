#pragma once

#include "UI/WindowInterface.h"
#include "Core/Misc/ConfigFile.h"

class SGMQuantConfigWindow : public IUIWindowInterface
{
public:
	SGMQuantConfigWindow() noexcept;

	bool IsWindowOpen() noexcept { return mIsWindowOpen; }

	void OnGui() noexcept;
	void Release() noexcept { delete this; }

private:
	bool mIsWindowOpen;
	std::shared_ptr<SConfigFile> mGMQuantConfig;
};