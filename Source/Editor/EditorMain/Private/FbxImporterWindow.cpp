#include "UI/UIModule.h"
#include "FbxImporterWindow.h"
#include "Core/Misc/Localize.h"
#include "Core/Util/UtilMacros.h"
#include "EditorMain/MessageBoxWindow.h"

#include "imgui.h"
#include "fbxsdk.h"

#pragma comment(lib, "libfbxsdk.lib")

DECLEAR_DEFAULT_WINDOW(SUIFbxImporterMenuWindow, , "FbxImporterMenu"); 

void SUIFbxImporterMenuWindow::OnGui() noexcept
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::MenuItem(u8"加载Mesh"))
		{
			GetUIModule()->FindOrAddWindow<SUIFbxImporterWindow>(u8"Mesh加载");
		}
		ImGui::EndMainMenuBar();
	}
}

void SUIFbxImporterWindow::OnGui() noexcept
{
	if (mIsWindowOpen == false)
		return;

	ImGui::Begin(u8"Mesh加载", &mIsWindowOpen);

	std::string fbxPath = mFbxPath.u8string();
	if (ImGui::InputText(u8"FBX路径", fbxPath, 512))
		mFbxPath = fbxPath;
	std::string destResourcePath = mDestResourcePath.u8string();
	if (ImGui::InputText(u8"生成资源路径", destResourcePath, 512))
		mDestResourcePath = destResourcePath;

	if (ImGui::Button(u8"加载"))
	{
		bool res = LoadFbxMesh(mFbxPath, mDestResourcePath);
		if (res == false)
		{
			GetUIModule()->FindOrAddWindow<SUIMessageBoxWindow>("", u8"加载失败", u8"加载失败");
		}
	}

	ImGui::End();
}

bool SUIFbxImporterWindow::LoadFbxMesh(const std::filesystem::path& fbxPath, const std::filesystem::path& destResourcePath) noexcept
{
	if (std::filesystem::exists(fbxPath) == false)
		return false;

	FbxManager* fbxManager = FbxManager::Create();
	FbxIOSettings* fbxIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(fbxIOSettings);

	FbxImporter* importer = FbxImporter::Create(fbxManager, "");
	if (importer->Initialize(Locale::ConvertWstringToString(fbxPath.wstring(), Locale::ECodePage::GBK).c_str(), -1, fbxManager->GetIOSettings()) == false)
		return false;

	FbxScene* fbxScene = FbxScene::Create(fbxManager, "Scene");
	importer->Import(fbxScene);


	importer->Destroy();

	return true;
}
