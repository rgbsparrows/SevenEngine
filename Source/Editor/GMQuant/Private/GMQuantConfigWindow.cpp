#include "Core/Math/Math.h"
#include "GMQuantConfigWindow.h"
#include "Core/ProgramConfiguation/BasicPath.h"

#include "imgui.h"
#include "imgui_internal.h"

SGMQuantConfigWindow::SGMQuantConfigWindow() noexcept
{
	mGMQuantConfig = SConfigFile::LoadConfigFile(SBasicPath::GetEngineConfigPath() / "GMQuantConfig.json");
}

void SGMQuantConfigWindow::OnGui() noexcept
{
	char textBuffer[512];

	ImGui::Begin(u8"量化配置", &mIsWindowOpen);
	
	std::string quantTerminalPath;
	mGMQuantConfig->GetValue("GMQuant", "TerminalPath", quantTerminalPath);
	strcpy_s(textBuffer, quantTerminalPath.c_str());
	if (ImGui::InputText(u8"量化终端路径", textBuffer, 512))
		mGMQuantConfig->SetValue("GMQuant", "TerminalPath", textBuffer);

	std::string userToken;
	mGMQuantConfig->GetValue("GMQuant", "UserToken", userToken);
	strcpy_s(textBuffer, userToken.c_str());
	if (ImGui::InputText(u8"用户Token", textBuffer, 512))
		mGMQuantConfig->SetValue("GMQuant", "UserToken", textBuffer);

	std::vector<std::string> strategyToken;
	mGMQuantConfig->GetValue("GMQuant", "StrategyToken", strategyToken);
	std::vector<std::string> strategyTokenDisplayName;
	mGMQuantConfig->GetValue("GMQuant", "StrategyTokenDisplayName", strategyTokenDisplayName);

	strategyTokenDisplayName.resize(strategyToken.size());
	bool strategyTokenChange = false;
	for (size_t i = 0, id = 0; i != strategyToken.size(); ++i, ++id)
	{
		ImGui::PushID(std::format(u8"策略{}", id).c_str());

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		strcpy_s(textBuffer, strategyTokenDisplayName[i].c_str());
		if (ImGui::InputText(u8"策略名称", textBuffer, 512))
		{
			strategyTokenDisplayName[i] = textBuffer;
			strategyTokenChange = true;
		}
		ImGui::PopItemWidth();

		ImGui::SameLine();
		strcpy_s(textBuffer, strategyToken[i].c_str());
		if (ImGui::InputText(u8"策略Token", textBuffer, 512))
		{
			strategyToken[i] = textBuffer;
			strategyTokenChange = true;
		}
		ImGui::PopItemWidth();

		ImGui::SameLine();
		if (ImGui::Button(u8"移除"))
		{
			strategyTokenDisplayName.erase(strategyTokenDisplayName.begin() + i);
			strategyToken.erase(strategyToken.begin() + i);
			strategyTokenChange = true;
			--i;
		}
		ImGui::PopItemWidth();

		ImGui::PopID();
	}

	if (ImGui::Button(u8"新增"))
	{
		strategyTokenDisplayName.push_back(std::string());
		strategyToken.push_back(std::string());
		strategyTokenChange = true;
	}

	if (strategyTokenChange)
	{
		mGMQuantConfig->SetValue("GMQuant", "StrategyToken", strategyToken);
		mGMQuantConfig->SetValue("GMQuant", "StrategyTokenDisplayName", strategyTokenDisplayName);
	}

	ImGui::End();
}
