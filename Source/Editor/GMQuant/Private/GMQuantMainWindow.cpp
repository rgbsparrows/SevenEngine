#include "GMQuantMainWindow.h"
#include "Core/ProgramConfiguation/BasicPath.h"
#include "UI/Imgui/imgui.h"

SGMQuantMainWindow::SGMQuantMainWindow() noexcept
{
	mGMQuantConfig = SConfigFile::LoadConfigFile(SBasicPath::GetEnginePluginPath() / "GMQuantConfig.json");
}

void SGMQuantMainWindow::OnGui() noexcept
{
	ImGui::Begin(u8"量化研究", &mIsWindowOpen);

	ImGui::End();
}

void SGMQuantMainWindow::Release() noexcept
{
	delete this;
}
