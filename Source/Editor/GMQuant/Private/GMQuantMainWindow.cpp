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
#include "Core/Class/ClassManager.h"

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
	ImGui::Begin(u8"�����б�");

	std::vector<SClassIdentifier> createQuantStrategyInfoList = GetGMQuantModuleImpl()->GetCreateQuantStrategyInfoList();

	for (size_t i = 0; i != createQuantStrategyInfoList.size(); ++i)
	{
		std::string_view strategyName = SClassManager::Get().GetClassObject(createQuantStrategyInfoList[i])->GetClassFullName();
		if (ImGui::Selectable(std::format(u8"{0}###{0}_{1}", strategyName, i).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			SQuantStrategyBase* strategy = SClassManager::Get().ConstructObject<SQuantStrategyBase>(createQuantStrategyInfoList[i]);
			if (strategy)
			{
				strategy->SetDisplayName(strategyName);
				mCurrentStrategy = strategy;
				mStrategyList.push_back(strategy);
			}
		}
	}

	ImGui::End();
}

void SGMQuantMainWindow::ShowCreatedStrategyList() noexcept
{
	ImGui::Begin(u8"�Ѽ��ز����б�");

	for (size_t i = 0; i != mStrategyList.size(); ++i)
	{
		if (ImGui::RadioButton(std::format("{0}###{0}_{1}", mStrategyList[i]->GetDisplayName().c_str(), i).c_str(), mCurrentStrategy == mStrategyList[i]))
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

void SGMQuantMainWindow::ShowGMQuantStartupWindow() noexcept
{
	ImGui::Begin(u8"����ģ�������˵�");

	if (ImGui::Button(u8"�����������ƽ̨"))
	{
		std::filesystem::path quantTerminalPath = mGMQuantConfig.GetTerminalPath();
		std::string userToken = mGMQuantConfig.GetUserToken();

		if (std::filesystem::exists(quantTerminalPath) == false || userToken.empty())
			GetUIModule()->FindOrAddWindow<SGMQuantConfigWindow>(u8"�������ô���");
		else
			GetGMQuantCoreModule()->StartupQuantitativeTerminal(quantTerminalPath, userToken);
	}

	ImGui::Separator();

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	if (ImGui::BeginCombo(u8"����Token", mGMQuantConfig.GetStrategyTokenDisplayName(mCurrentStrategyToken).c_str()))
	{
		std::vector<std::string> strategyTokenList = mGMQuantConfig.GetStrategyTokenList();

		for (size_t i = 0; i != strategyTokenList.size(); ++i)
		{
			std::string displayName = mGMQuantConfig.GetStrategyTokenDisplayName(strategyTokenList[i]);

			if (ImGui::Selectable(std::format(u8"{0}###{0}_{1}", displayName.c_str(), i).c_str()))
			{
				mCurrentStrategyToken = strategyTokenList[i];
			}
		}

		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::SameLine();
	if (ImGui::Button(u8"ִ�в���"))
	{
		if (mCurrentStrategy != nullptr && mCurrentStrategy->AvilableForExecute())
		{
			GetGMQuantCoreModule()->ExecuteBacktestQuantStrategy(mCurrentStrategy, mCurrentStrategyToken);
		}
	}

	ImGui::End();
}

void SGMQuantMainWindow::ShowStatsWindow() noexcept
{
	ImGui::Begin(u8"ͳ�����");

	if (mCurrentStrategy != nullptr)
	{
		switch (mCurrentStrategy->GetStrategyExecuteState())
		{
		case EStrategyExecuteState::Unexecuted: ImGui::Text(u8"����δִ��"); break;
		case EStrategyExecuteState::WaitingForExecute: ImGui::Text(u8"�ȴ�ִ��"); break;
		case EStrategyExecuteState::Executing: ImGui::Text(u8"����ִ����"); break;
		case EStrategyExecuteState::Executed: ImGui::Text(u8"������ִ��"); break;
		}

		ImGui::Separator();

		mCurrentStrategy->StatsGui();
	}
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
