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
		if (ImGui::BeginMenu(u8"配置"))
		{
			if (ImGui::MenuItem(u8"量化配置"))
				GetUIModule()->FindOrAddWindow<SGMQuantConfigWindow>(u8"量化配置窗口");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(u8"窗口"))
		{

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin(u8"量化投资", &mIsWindowOpen);
	ImGui::DockSpace(ImGui::GetID(u8"量化投资Dockspace"));

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
	ImGui::Begin(u8"策略列表");

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
	ImGui::Begin(u8"已加载策略列表");

	for (size_t i = 0; i != mStrategyList.size(); ++i)
	{
		if (ImGui::RadioButton(std::format("{0}###{0}_{1}", mStrategyList[i]->GetDisplayName().c_str(), i).c_str(), mCurrentStrategy == mStrategyList[i]))
			mCurrentStrategy = mStrategyList[i];
	}

	ImGui::End();
}

void SGMQuantMainWindow::ShowSteagyMainWindow() noexcept
{
	ImGui::Begin(u8"策略主窗口");

	if (mCurrentStrategy != nullptr)
		mCurrentStrategy->Gui();
	else
		ImGui::Text(u8"无选中策略");

	ImGui::End();
}

void SGMQuantMainWindow::ShowGMQuantStartupWindow() noexcept
{
	ImGui::Begin(u8"量化模块启动菜单");

	if (ImGui::Button(u8"启动掘金量化平台"))
	{
		std::filesystem::path quantTerminalPath = mGMQuantConfig.GetTerminalPath();
		std::string userToken = mGMQuantConfig.GetUserToken();

		if (std::filesystem::exists(quantTerminalPath) == false || userToken.empty())
			GetUIModule()->FindOrAddWindow<SGMQuantConfigWindow>(u8"量化配置窗口");
		else
			GetGMQuantCoreModule()->StartupQuantitativeTerminal(quantTerminalPath, userToken);
	}

	ImGui::Separator();

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	if (ImGui::BeginCombo(u8"策略Token", mGMQuantConfig.GetStrategyTokenDisplayName(mCurrentStrategyToken).c_str()))
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
	if (ImGui::Button(u8"执行策略"))
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
	ImGui::Begin(u8"统计面板");

	if (mCurrentStrategy != nullptr)
	{
		switch (mCurrentStrategy->GetStrategyExecuteState())
		{
		case EStrategyExecuteState::Unexecuted: ImGui::Text(u8"策略未执行"); break;
		case EStrategyExecuteState::WaitingForExecute: ImGui::Text(u8"等待执行"); break;
		case EStrategyExecuteState::Executing: ImGui::Text(u8"策略执行中"); break;
		case EStrategyExecuteState::Executed: ImGui::Text(u8"策略已执行"); break;
		}

		ImGui::Separator();

		mCurrentStrategy->StatsGui();
	}
	else
		ImGui::Text(u8"无选中策略");

	ImGui::End();
}

void SGMQuantMainWindow::ShowSteagyParameterWindow() noexcept
{
	ImGui::Begin(u8"策略参数");

	if (mCurrentStrategy != nullptr)
		mCurrentStrategy->ParameterGui();
	else
		ImGui::Text(u8"无选中策略");

	ImGui::End();
}
