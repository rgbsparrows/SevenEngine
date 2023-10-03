#include "Core/Math/Math.h"
#include "Core/Misc/Localize.h"
#include "GMQuantConfigWindow.h"
#include "Core/ProgramConfiguation/BasicPath.h"

#include "imgui.h"
#include "imgui_internal.h"

void SGMQuantConfigWindow::OnGui() noexcept
{
	char textBuffer[512];

	ImGui::Begin(u8"��������", &mIsWindowOpen);
	
	std::filesystem::path quantTerminalPath = mGMQuantConfig.GetTerminalPath();
	strcpy_s(textBuffer, quantTerminalPath.u8string().c_str());
	if (ImGui::InputText(u8"�����ն�·��", textBuffer, 512))
		mGMQuantConfig.SetTerminalPath(Locale::ConvertStringToWstring(textBuffer));

	std::string userToken = mGMQuantConfig.GetUserToken();
	strcpy_s(textBuffer, userToken.c_str());
	if (ImGui::InputText(u8"�û�Token", textBuffer, 512))
		mGMQuantConfig.SetUserToken(userToken);

	std::vector<std::string> strategyTokenList = mGMQuantConfig.GetStrategyTokenList();
	for (size_t i = 0, id = 0; i != strategyTokenList.size(); ++i, ++id)
	{
		const std::string& strategyToken = strategyTokenList[i];
		const std::string& displayName = mGMQuantConfig.GetStrategyTokenDisplayName(strategyToken);

		ImGui::PushID(std::format(u8"����{}", id).c_str());

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		strcpy_s(textBuffer, displayName.c_str());
		if (ImGui::InputText(u8"��������", textBuffer, 512))
			mGMQuantConfig.SetStrategyToken(strategyToken, textBuffer);
		ImGui::PopItemWidth();

		ImGui::SameLine();
		strcpy_s(textBuffer, strategyToken.c_str());
		if (ImGui::InputText(u8"����Token", textBuffer, 512))
		{
			mGMQuantConfig.RemoveStrategyToken(strategyToken);
			mGMQuantConfig.SetStrategyToken(textBuffer, displayName);
		}
		ImGui::PopItemWidth();

		ImGui::SameLine();
		if (ImGui::Button(u8"�Ƴ�"))
			mGMQuantConfig.RemoveStrategyToken(strategyToken);
		ImGui::PopItemWidth();

		ImGui::PopID();
	}

	if (ImGui::Button(u8"����"))
	{
		mGMQuantConfig.SetStrategyToken("", u8"δ��������Token");
	}

	ImGui::End();
}
