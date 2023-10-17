#include "Core/Util/Algorithm.h"
#include "Core/Misc/ConfigFile.h"

#include <fstream>

std::vector<std::weak_ptr<SConfigFile>> SConfigFile::GConfigFiles;

std::shared_ptr<SConfigFile> SConfigFile::LoadConfigFile(const std::filesystem::path& _configFilePath) noexcept
{
	EraseIf(GConfigFiles, [](const std::weak_ptr<SConfigFile>& _configFile) { return _configFile.expired(); });

	auto it = FindIf(GConfigFiles, [&](const std::weak_ptr<SConfigFile>& _configFile) { return _configFile.lock()->GetConfigFilePath() == _configFilePath; });

	if (it != GConfigFiles.end()) return it->lock();

	auto configFile = std::shared_ptr<SConfigFile>(new SConfigFile(_configFilePath));
	GConfigFiles.push_back(configFile);

	return configFile;
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, int32_t& _value) const noexcept
{
	if (mConfigJsonData.is_null() || !mConfigJsonData.contains(_category) || !mConfigJsonData[_category].contains(_key) || !mConfigJsonData[_category][_key].is_number_integer())
		return false;

	_value = mConfigJsonData[_category][_key].get<int32_t>();
	return true;
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, float& _value) const noexcept
{
	if (mConfigJsonData.is_null() || !mConfigJsonData.contains(_category) || !mConfigJsonData[_category].contains(_key) || !mConfigJsonData[_category][_key].is_number())
		return false;

	_value = mConfigJsonData[_category][_key].get<float>();
	return true;
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, std::string& _value) const noexcept
{
	if (mConfigJsonData.is_null() || !mConfigJsonData.contains(_category) || !mConfigJsonData[_category].contains(_key) || !mConfigJsonData[_category][_key].is_string())
		return false;

	_value = mConfigJsonData[_category][_key].get<std::string>();
	return true;
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, bool& _value) const noexcept
{
	if (mConfigJsonData.is_null() || !mConfigJsonData.contains(_category) || !mConfigJsonData[_category].contains(_key) || !mConfigJsonData[_category][_key].is_boolean())
		return false;

	_value = mConfigJsonData[_category][_key].get<bool>();
	return true;
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, std::vector<std::string>& _value) const noexcept
{
	if (mConfigJsonData.is_null() || !mConfigJsonData.contains(_category) || !mConfigJsonData[_category].contains(_key) || !mConfigJsonData[_category][_key].is_array())
		return false;

	_value.clear();
	for (auto& value : mConfigJsonData[_category][_key])
	{
		if (!value.is_string()) return false;
		_value.push_back(value.get<std::string>());
	}

	return true;
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, nlohmann::json& _value) const noexcept
{
	if (mConfigJsonData.is_null() || !mConfigJsonData.contains(_category) || !mConfigJsonData[_category].contains(_key))
		return false;

	_value = mConfigJsonData[_category][_key];
	return true;
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, const std::filesystem::path& _value) noexcept
{
	SetValue(_category, _key, _value.u8string().c_str());
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, const char* _value) noexcept
{
	SetValue(_category, _key, std::string(_value));
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, int32_t _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
	SaveConfig();
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, float _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
	SaveConfig();
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, const std::string& _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
	SaveConfig();
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, bool _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
	SaveConfig();
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, const std::vector<std::string>& _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
	SaveConfig();
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, const nlohmann::json& _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
	SaveConfig();
}

void SConfigFile::SaveConfig() noexcept
{
	std::ofstream ofs(GetConfigFilePath());
	if (ofs.is_open())
		ofs << std::setw(4) << mConfigJsonData << std::endl;
}

SConfigFile::SConfigFile(const std::filesystem::path& _configFile) noexcept
{
	mConfigFilePath = _configFile;
	std::ifstream ifs(GetConfigFilePath());
	if (ifs.is_open())
		mConfigJsonData = nlohmann::json::parse(ifs);
}
