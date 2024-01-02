#include "EditorMain/MessageBoxWindow.h"
#include "Core/Util/TemplateUtil.h"

#include <format>
#include "imgui.h"
#include "UI/UIModule.h"

void SUIMessageBoxWindow::OnGui() noexcept
{
	ImGui::Begin(std::format(u8"{0}###{1}", mTitle, PointerToInt(this)).c_str(), &mIsWindowOpen);
	ImGui::Text(mDisplayText.c_str());
	ImGui::End();
}
