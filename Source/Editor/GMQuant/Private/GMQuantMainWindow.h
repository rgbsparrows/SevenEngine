#pragma once

#include "UI/WindowInterface.h"
#include "Core/Misc/ConfigFile.h"

class SQuantStrategyBase;

class SGMQuantMainWindow : public IUIWindowInterface
{
public:
	SGMQuantMainWindow() noexcept;

	bool IsWindowOpen() noexcept override { return mIsWindowOpen; }

	void OnGui() noexcept override;
	void Release() noexcept override;

protected:
	void ShowAllStrategyList() noexcept;
	void ShowCreatedStrategyList() noexcept;
	void ShowSteagyMainWindow() noexcept;
	void ShowSteagyParameterWindow() noexcept;
	void ShowStatsWindow() noexcept;
	void ShowGMQuantStartupWindow() noexcept;

private:
	std::shared_ptr<SConfigFile> mGMQuantConfig;

	std::vector<SQuantStrategyBase*> mStrategyList;
	SQuantStrategyBase* mCurrentStrategy = nullptr;

	bool mIsWindowOpen = true;
};