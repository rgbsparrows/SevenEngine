#pragma once

#include "UI/WindowInterface.h"
#include "Core/Misc/ConfigFile.h"
#include "GMQuantConfigFile.h"

class SGMQuantConfigWindow : public IUIWindowInterface
{
public:
	bool IsWindowOpen() noexcept { return mIsWindowOpen; }

	void OnGui() noexcept;
	void Release() noexcept { delete this; }

private:
	bool mIsWindowOpen;
	SGMQuantConfigFile mGMQuantConfig;
};