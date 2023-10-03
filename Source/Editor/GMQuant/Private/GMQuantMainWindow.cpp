#include "UI/UIModule.h"
#include "GMQuantMainWindow.h"
#include "GMQuantModuleImpl.h"
#include "GMQuantConfigWindow.h"
#include "GMQuantCore/QuantStrategy.h"
#include "GMQuantCore/GMQuantCoreModule.h"
#include "GMQuantCore/QuantStrategyContext.h"
#include "Core/ProgramConfiguation/BasicPath.h"

#include <chrono>
#include "imgui.h"
#include "imgui_internal.h"

SGMQuantMainWindow::SGMQuantMainWindow() noexcept
{
	mGMQuantConfig = SConfigFile::LoadConfigFile(SBasicPath::GetEngineConfigPath() / "GMQuantConfig.json");
}

void SGMQuantMainWindow::OnGui() noexcept
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(u8"����"))
		{
			if (ImGui::MenuItem(u8"��������"))
				GetUIModule()->FindOrAddWindow<SGMQuantConfigWindow>(u8"�������ô���");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(u8"����"))
		{

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin(u8"����Ͷ��", &mIsWindowOpen);
	ImGui::DockSpace(ImGui::GetID(u8"����Ͷ��Dockspace"));

	ShowAllStrategyList();
	ShowCreatedStrategyList();
	ShowSteagyMainWindow();
	ShowSteagyParameterWindow();
	ShowStatsWindow();
	ShowGMQuantStartupWindow();

	ImGui::End();
}

void SGMQuantMainWindow::Release() noexcept
{
	delete this;
}

void SGMQuantMainWindow::ShowAllStrategyList() noexcept
{
	ImGui::Begin(u8"�����б�", nullptr, ImGuiWindowFlags_NoTitleBar);

	std::vector<SCreateQuantStrategyInfo> createQuantStrategyInfoList = GetGMQuantModuleImpl()->GetCreateQuantStrategyInfoList();

	if (ImGui::BeginTable("�����б�Table", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		for (size_t i = 0; i != createQuantStrategyInfoList.size(); ++i)
		{
			ImGui::TableNextColumn();
			ImGui::Text(createQuantStrategyInfoList[i].mStrategyName.c_str());

			ImGui::TableNextColumn();
			if (ImGui::Button(std::format(u8"����###����{0}_{1}", createQuantStrategyInfoList[i].mStrategyName.c_str(), i).c_str()))
			{
				SQuantStrategyBase* strategy = createQuantStrategyInfoList[i].mCreateQuantStrategy();
				if (strategy)
				{
					strategy->SetDisplayName(createQuantStrategyInfoList[i].mStrategyName);
					mStrategyList.push_back(strategy);
				}
			}
		}

		ImGui::EndTable();
	}

	ImGui::End();
}

void SGMQuantMainWindow::ShowCreatedStrategyList() noexcept
{
	ImGui::Begin(u8"�Ѽ��ز����б�");

	for (size_t i = 0; i != mStrategyList.size(); ++i)
	{
		if (ImGui::Selectable(std::format("{0}###{0}_{1}", mStrategyList[i]->GetDisplayName().c_str(), i).c_str(), mCurrentStrategy == mStrategyList[i], ImGuiSelectableFlags_AllowDoubleClick))
			mCurrentStrategy = mStrategyList[i];
	}

	ImGui::End();
}

void SGMQuantMainWindow::ShowSteagyMainWindow() noexcept
{
	ImGui::Begin(u8"����������");

	if (mCurrentStrategy != nullptr)
		mCurrentStrategy->Gui();
	else
		ImGui::Text(u8"��ѡ�в���");

	ImGui::End();
}

void SGMQuantMainWindow::ShowSteagyParameterWindow() noexcept
{
	ImGui::Begin(u8"���Բ���");

	if (mCurrentStrategy != nullptr)
		mCurrentStrategy->ParameterGui();
	else
		ImGui::Text(u8"��ѡ�в���");

	ImGui::End();
}

void SGMQuantMainWindow::ShowStatsWindow() noexcept
{
	ImGui::Begin(u8"ͳ�����");

	if (mCurrentStrategy != nullptr)
		mCurrentStrategy->StatsGui();
	else
		ImGui::Text(u8"��ѡ�в���");

	ImGui::End();
}

void SGMQuantMainWindow::ShowGMQuantStartupWindow() noexcept
{
	ImGui::Begin(u8"����ģ�������˵�");

	if (ImGui::Button(u8"�����������ƽ̨"))
	{
		std::string quantTerminalPath;
		mGMQuantConfig->GetValue("GMQuant", "TerminalPath", quantTerminalPath);

		GetGMQuantCoreModule()->StartupQuantitativeTerminal(quantTerminalPath);
	}

	if (ImGui::Button(u8"�����û�Token"))
	{
		std::string userToken;
		mGMQuantConfig->GetValue("GMQuant", "UserToken", userToken);

		GetGMQuantCoreModule()->SetUserToken(userToken);
	}

	ImGui::End();
}
