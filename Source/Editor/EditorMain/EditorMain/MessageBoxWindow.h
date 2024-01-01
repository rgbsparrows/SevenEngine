#pragma once

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
