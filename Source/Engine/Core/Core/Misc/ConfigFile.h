#pragma once

#include "Core/Misc/json.hpp"

#include <map>
#include <string>
#include <filesystem>

class SConfigFile
{
public:
	static std::shared_ptr<SConfigFile> LoadConfigFile(const std::filesystem::path& _configFile) noexcept;

	std::filesystem::path GetConfigFilePath() const noexcept {return mConfigFilePath; }

	bool GetValue(const std::string& _category, const std::string& _key, int32_t& _value) const noexcept;
	bool GetValue(const std::string& _category, const std::string& _key, float& _value) const noexcept;
	bool GetValue(const std::string& _category, const std::string& _key, std::string& _value) const noexcept;
	bool GetValue(const std::string& _category, const std::string& _key, bool& _value) const noexcept;
	bool GetValue(const std::string& _category, const std::string& _key, std::vector<std::string>& _value) const noexcept;

	void SetValue(const std::string& _category, const std::string& _key, int32_t _value) noexcept;
	void SetValue(const std::string& _category, const std::string& _key, float _value) noexcept;
	void SetValue(const std::string& _category, const std::string& _key, const std::string& _value) noexcept;
	void SetValue(const std::string& _category, const std::string& _key, bool _value) noexcept;
	void SetValue(const std::string& _category, const std::string& _key, const std::vector<std::string>& _value) noexcept;

	void SaveConfig() noexcept;

private:
	SConfigFile(const std::filesystem::path& _configFile) noexcept;
	std::filesystem::path mConfigFilePath;
	nlohmann::json mConfigJsonData;
	static std::vector<std::weak_ptr<SConfigFile>> GConfigFiles;
};
