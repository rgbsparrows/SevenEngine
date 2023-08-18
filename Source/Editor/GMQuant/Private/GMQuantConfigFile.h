#pragma once

#include "Core/Misc/json.hpp"
#include "Core/Misc/ConfigFile.h"
#include "Core/ProgramConfiguation/BasicPath.h"

class SGMQuantConfigFile
{
public:
	SGMQuantConfigFile() noexcept
	{
		mGMQuantConfig = SConfigFile::LoadConfigFile(SBasicPath::GetEngineConfigPath() / "GMQuantConfig.json");
	}

	std::filesystem::path GetTerminalPath() const noexcept
	{
		std::string quantTerminalPath;
		mGMQuantConfig->GetValue("GMQuant", "TerminalPath", quantTerminalPath);
		return quantTerminalPath;
	}

	void SetTerminalPath(const std::filesystem::path& _terminalPath) noexcept
	{
		mGMQuantConfig->SetValue("GMQuant", "TerminalPath", _terminalPath.u8string());
	}

	std::string GetUserToken() const noexcept
	{
		std::string userToken;
		mGMQuantConfig->GetValue("GMQuant", "UserToken", userToken);
		return userToken;
	}

	void SetUserToken(const std::string& _userToken) noexcept
	{
		mGMQuantConfig->SetValue("GMQuant", "UserToken", _userToken);
	}

	std::vector<std::string> GetStrategyTokenList() const noexcept
	{
		nlohmann::json strategyTokenList;
		mGMQuantConfig->GetValue("GMQuant", "StrategyToken", strategyTokenList);

		std::vector<std::string> strategyToken;
		for (size_t i = 0; i != strategyTokenList.size(); ++i)
		{
			strategyToken.push_back(strategyTokenList[i]["Token"].get<std::string>());
		}

		return strategyToken;
	}

	std::string GetStrategyTokenDisplayName(const std::string& _token) const noexcept
	{
		nlohmann::json strategyTokenList;

		std::vector<std::string> strategyToken;
		mGMQuantConfig->GetValue("GMQuant", "StrategyToken", strategyTokenList);

		for (size_t i = 0; i != strategyTokenList.size(); ++i)
		{
			if (strategyTokenList[i]["Token"].get<std::string>() == _token)
			{
				return strategyTokenList[i]["DisplayName"].get<std::string>();
			}
		}

		return u8"ÎÞÐ§²ßÂÔ";
	}

	void SetStrategyToken(const std::string& _token, const std::string& _displayName) noexcept
	{
		nlohmann::json strategyTokenList;
		mGMQuantConfig->GetValue("GMQuant", "StrategyToken", strategyTokenList);

		for (size_t i = 0; i != strategyTokenList.size(); ++i)
		{
			if (strategyTokenList[i]["Token"].get<std::string>() == _token)
			{
				strategyTokenList[i]["DisplayName"] = _displayName;
				mGMQuantConfig->SetValue("GMQuant", "StrategyToken", strategyTokenList);
				return;
			}
		}

		strategyTokenList.push_back({ {"Token", _token}, {"DisplayName", _displayName} });
		mGMQuantConfig->SetValue("GMQuant", "StrategyToken", strategyTokenList);
	}

	void RemoveStrategyToken(const std::string& _token) noexcept
	{
		nlohmann::json strategyTokenList;
		mGMQuantConfig->GetValue("GMQuant", "StrategyToken", strategyTokenList);

		for (size_t i = 0; i != strategyTokenList.size(); ++i)
		{
			if (strategyTokenList[i]["Token"].get<std::string>() == _token)
			{
				strategyTokenList.erase(i);
				mGMQuantConfig->SetValue("GMQuant", "StrategyToken", strategyTokenList);
				return;
			}
		}
	}

private:
	std::shared_ptr<SConfigFile> mGMQuantConfig;
};
