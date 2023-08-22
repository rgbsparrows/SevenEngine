#pragma once

#include "GMQuantConfigFile.h"
#include "UI/WindowInterface.h"
#include "Core/Misc/ConfigFile.h"

class SQuantStrategyBase;

class SGMQuantMainWindow : public IUIWindowInterface
{
public:
	bool IsWindowOpen() noexcept override { return mIsWindowOpen; }

	void OnGui() noexcept override;
	void Release() noexcept override;

protected:
	void ShowAllStrategyList() noexcept;
	void ShowCreatedStrategyList() noexcept;
	void ShowSteagyMainWindow() noexcept;
	void ShowGMQuantStartupWindow() noexcept;
	void ShowStatsWindow() noexcept;
	void ShowSteagyParameterWindow() noexcept;

private:
	SGMQuantConfigFile mGMQuantConfig;

	std::vector<SQuantStrategyBase*> mStrategyList;
	SQuantStrategyBase* mCurrentStrategy = nullptr;

	std::string mCurrentStrategyToken;

	bool mIsWindowOpen = true;
};