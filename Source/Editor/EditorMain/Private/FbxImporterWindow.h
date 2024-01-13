#pragma once

#include "UI/WindowInterface.h"
#include "Resource/ResourceType/StaticMeshResource.h"

#include "fbxsdk.h"
#include <intsafe.h>
#include <filesystem>

class SUIFbxImporterMenuWindow : public SUILongLivedWindow
{
public:
	bool IsWindowOpen() noexcept { return true; }

	void OnGui() noexcept;
	void Release() noexcept {}
};

struct SLoadedStaticMeshInfo
{
	std::string mName;
	std::filesystem::path mSavePath;
	SStaticMeshRP mStaticMesh;
};

class SUIFbxImporterWindow : public SUIStandardWindow
{
public:
	void OnGui() noexcept;
	void Release() noexcept {}

private:
	bool LoadFbxMesh(const std::filesystem::path& fbxPath, const std::filesystem::path& destResourcePath) noexcept;

	void RecursiveProcessFbxNode(fbxsdk::FbxNode* _node);
	void ProcessFbxMesh(fbxsdk::FbxMesh* _mesh);

private:
	void MergeRepeatedVertex(std::vector<SFullVertex>& _vertexs, std::vector<SMeshTriangle>& _triangles, size_t _maxSearchSpan = SIZE_T_ERROR);
	void ApplyTriangleIndexOffset(std::vector<SMeshTriangle>& _triangles, uint32_t _offset) noexcept;

private:
	std::filesystem::path mFbxPath = R"(C:\Users\76546\Desktop\SM_TableRound.FBX)";
	std::filesystem::path mDestResourcePath;

	std::vector<SLoadedStaticMeshInfo> mLoadedMesh;
};