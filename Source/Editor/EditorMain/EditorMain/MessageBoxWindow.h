#pragma once

#include "UI/UIModule.h"
#include "UI/WindowInterface.h"

#include <string>

class SUIMessageBoxWindow : public SUIStandardWindow
{
public:
	SUIMessageBoxWindow(const std::string& _title, const std::string& _displayText)
		:mTitle(_title), mDisplayText(_displayText)
	{
	}

	void OnGui() noexcept;

private:
	std::string mTitle;
	std::string mDisplayText;;
};

void MessageBox(const std::string& _title, const std::string& _displayText) noexcept
{
	GetUIModule()->FindOrAddWindow<SUIMessageBoxWindow>(std::string(), _title, _displayText);
}
