#pragma once

#include "UI/WindowInterface.h"

#include <filesystem>

class SUIFbxImporterMenuWindow : public SUILongLivedWindow
{
public:
	bool IsWindowOpen() noexcept { return true; }

	void OnGui() noexcept;
	void Release() noexcept {}
};

class SUIFbxImporterWindow : public SUIStandardWindow
{
public:
	void OnGui() noexcept;
	void Release() noexcept {}

private:
	bool LoadFbxMesh(const std::filesystem::path& fbxPath, const std::filesystem::path& destResourcePath) noexcept;

private:
	std::filesystem::path mFbxPath;
	std::filesystem::path mDestResourcePath;
};