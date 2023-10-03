#pragma once

#include "UI/WindowInterface.h"
#include "Core/Misc/ConfigFile.h"

class SGMQuantMainWindow : public IUIWindowInterface
{
public:
	SGMQuantMainWindow() noexcept;

	bool IsWindowOpen() noexcept override { return mIsWindowOpen; }

	void OnGui() noexcept override;
	void Release() noexcept override;

private:
	std::shared_ptr<SConfigFile> mGMQuantConfig;

	bool mIsWindowOpen = true;
};