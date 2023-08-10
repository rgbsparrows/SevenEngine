#include "Core/Util/Algorithm.h"
#include "Core/Misc/ConfigFile.h"

TSharedPtr<SConfigFile> SConfigFile::LoadConfigFile(const std::filesystem::path& _configFilePath) noexcept
{
	Erase(GConfigFiles, TWeakPtr<SConfigFile>());

	auto it = FindIf(GConfigFiles, [&](const TWeakPtr<SConfigFile>& _configFile) { return _configFile->GetConfigFilePath() == _configFilePath; });

	if (it != GConfigFiles.end()) return it->lock();

	auto configFile = TWeakPtr<SConfigFile>(new SConfigFile(_configFilePath));
	GConfigFiles.push_back(configFile);

	return configFile.lock();
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, int32_t& _value) const noexcept
{
	if (mConfigJsonData[_category].is_null() || !mConfigJsonData[_category][_key].is_number_integer())
		return false;

	_value = mConfigJsonData[_category][_key].get<int32_t>();
	return true;
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, float& _value) const noexcept
{
	if (mConfigJsonData[_category].is_null() || !mConfigJsonData[_category][_key].is_number())
		return false;

	_value = mConfigJsonData[_category][_key].get<float>();
	return true;
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, std::string& _value) const noexcept
{
	if (mConfigJsonData[_category].is_null() || !mConfigJsonData[_category][_key].is_string())
		return false;

	_value = mConfigJsonData[_category][_key].get<std::string>();
	return true;
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, bool& _value) const noexcept
{
	if (mConfigJsonData[_category].is_null() || !mConfigJsonData[_category][_key].is_boolean())
		return false;

	_value = mConfigJsonData[_category][_key].get<bool>();
	return true;
}

bool SConfigFile::GetValue(const std::string& _category, const std::string& _key, std::vector<std::string>& _value) const noexcept
{
	if (mConfigJsonData[_category].is_null() || !mConfigJsonData[_category][_key].is_array())
		return false;

	_value.clear();
	for (auto& value : mConfigJsonData[_category][_key])
	{
		if (!value.is_string()) return false;
		_value.push_back(value.get<std::string>());
	}

	return true;
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, int32_t _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, float _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, const std::string& _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, bool _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
}

void SConfigFile::SetValue(const std::string& _category, const std::string& _key, const std::vector<std::string>& _value) noexcept
{
	mConfigJsonData[_category][_key] = _value;
}

void SConfigFile::SaveConfig() noexcept
{
	std::ofstream ofs(GetConfigFilePath());
	ofs << mConfigJsonData << std::endl;
}

SConfigFile::SConfigFile(const std::filesystem::path& _configFile) noexcept
{
	mConfigFilePath = _configFile;
	std::ifstream ifs(GetConfigFilePath());
	mConfigJsonData = nlohmann::json::parse(ifs);
}
