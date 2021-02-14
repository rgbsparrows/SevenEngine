#include "Core/CommandLine/CommandLine.h"
#include "WindowsPlatform/WindowsPlatformApi.h"

std::wstring SCommandLine::GetCommandLine() noexcept
{
	return SWindowsPlatformApi::GetCommandLine();
}

std::pair<bool, std::wstring_view> SCommandLine::GetRawArgumentValue(std::wstring_view _key) noexcept
{
	static auto skipWhiteSpcae = [](std::wstring_view& _commandLine)
	{
		size_t pos = _commandLine.find_first_not_of(L' ');
		if (pos != std::wstring_view::npos)
			_commandLine.remove_prefix(pos);
	};

	static auto getNextToken = [](std::wstring_view& _commandLine) {
		if (_commandLine.empty()) return std::wstring_view();

		size_t tokenEnd = 0;
		for (bool isInQuotes = false; tokenEnd != _commandLine.size() && (isInQuotes || _commandLine[tokenEnd] != L' '); ++tokenEnd)
			if (_commandLine[tokenEnd] == L'\"') isInQuotes = !isInQuotes;

		std::wstring_view token = _commandLine.substr(0, tokenEnd);
		_commandLine.remove_prefix(tokenEnd);

		return token;
	};

	static std::wstring rawCommandLine = GetCommandLine();
	std::wstring_view restCommandLine = rawCommandLine;
	//第一个参数为exe路径，无用
	getNextToken(restCommandLine);

	while (true)
	{
		skipWhiteSpcae(restCommandLine);
		std::wstring_view token = getNextToken(restCommandLine);

		if (token.empty()) break;

		size_t keyEnd = token.find(L'=');
		if (keyEnd == std::wstring_view::npos) continue;

		std::wstring_view key = token.substr(0, keyEnd);

		if (key == _key)
		{
			token.remove_prefix(keyEnd + 1);
			std::wstring_view value = token;

			if (value.size() >= 2 && value.front() == L'\"' && value.back() == L'\"')
			{
				value.remove_prefix(1);
				value.remove_suffix(1);
			}
			return std::make_pair(true, value);
		}
	}

	return std::make_pair(false, std::wstring_view());
}
