#include "Core/Misc/Localize.h"
#include "Core/Misc/windowsEx.h"
#include "Core/Misc/CommandLine.h"


const std::string& SCommandLine::GetCommandLine() noexcept
{
	static std::string commandLine = Locale::ConvertWstringToString(::GetCommandLineW());
	return commandLine;
}

std::pair<bool, std::string_view> SCommandLine::GetRawArgumentValue(std::string_view _key) noexcept
{
	static auto skipWhiteSpcae = [](std::string_view& _commandLine)
	{
		size_t pos = _commandLine.find_first_not_of(L' ');
		if (pos != std::string_view::npos)
			_commandLine.remove_prefix(pos);
	};

	static auto getNextToken = [](std::string_view& _commandLine) {
		if (_commandLine.empty()) return std::string_view();

		size_t tokenEnd = 0;
		for (bool isInQuotes = false; tokenEnd != _commandLine.size() && (isInQuotes || _commandLine[tokenEnd] != L' '); ++tokenEnd)
			if (_commandLine[tokenEnd] == L'\"') isInQuotes = !isInQuotes;

		std::string_view token = _commandLine.substr(0, tokenEnd);
		_commandLine.remove_prefix(tokenEnd);

		return token;
	};

	static std::string rawCommandLine = GetCommandLine();
	std::string_view restCommandLine = rawCommandLine;
	//第一个参数为exe路径，无用
	getNextToken(restCommandLine);

	while (true)
	{
		skipWhiteSpcae(restCommandLine);
		std::string_view token = getNextToken(restCommandLine);

		if (token.empty()) break;

		size_t keyEnd = token.find(L'=');
		if (keyEnd == std::string_view::npos) continue;

		std::string_view key = token.substr(0, keyEnd);

		if (key == _key)
		{
			token.remove_prefix(keyEnd + 1);
			std::string_view value = token;

			if (value.size() >= 2 && value.front() == L'\"' && value.back() == L'\"')
			{
				value.remove_prefix(1);
				value.remove_suffix(1);
			}
			return std::make_pair(true, value);
		}
	}

	return std::make_pair(false, std::string_view());
}
