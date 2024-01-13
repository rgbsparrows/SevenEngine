#include "UIWindow.h"
#include "Core/Misc/Localize.h"
#include "Render/RenderModule.h"
#include "Render/RenderCommandList.h"

#include <format>

void SUIInternalWindow::Init(ImGuiViewport* _viewport)
{
	DWORD wndStyle = 0;
	DWORD wndExStyle = 0;
	SUIInternalWindow* parrentWindow = nullptr;
	Math::SFloatRect windowRect;
	std::string windowName;

	if (ImGui::GetMainViewport() == _viewport)
	{
		windowRect.mLeftOn[0] = 100;
		windowRect.mLeftOn[1] = 100;

		windowRect.mRightDwon[0] = 1280 + 100;
		windowRect.mRightDwon[1] = 800 + 100;

		windowName = u8"SevenEngine";

		wndStyle = WS_POPUP;
		wndExStyle = WS_EX_APPWINDOW;
	}
	else
	{
		if (_viewport->Flags & ImGuiViewportFlags_NoDecoration)
			wndStyle |= WS_POPUP;
		else
			wndStyle |= WS_OVERLAPPEDWINDOW;

		if (_viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon)
			wndExStyle |= WS_EX_TOOLWINDOW;
		else
			wndExStyle |= WS_EX_APPWINDOW;

		if (_viewport->Flags & ImGuiViewportFlags_TopMost)
			wndExStyle |= WS_EX_TOPMOST;

		if (_viewport->ParentViewportId != 0)
		{
			if (ImGuiViewport* parrentViewport = ImGui::FindViewportByID(_viewport->ParentViewportId))
				parrentWindow = static_cast<SUIInternalWindow*>(parrentViewport->PlatformUserData);
		}

		windowRect.mLeftOn[0] = _viewport->Pos.x;
		windowRect.mLeftOn[1] = _viewport->Pos.y;

		windowRect.mRightDwon[0] = _viewport->Pos.x + _viewport->Size.x;
		windowRect.mRightDwon[1] = _viewport->Pos.y + _viewport->Size.y;

		windowName = std::format("SevenEngine [{:p}]", static_cast<void*>(this));
	}

	RECT rect = {
		static_cast<int>(windowRect.mLeftOn[0]),
		static_cast<int>(windowRect.mLeftOn[1]),
		static_cast<int>(windowRect.mRightDwon[0]),
		static_cast<int>(windowRect.mRightDwon[1])
	};

	::AdjustWindowRectEx(&rect, wndStyle, FALSE, wndExStyle);

	HWND parrentHwnd = parrentWindow == nullptr ? nullptr : parrentWindow->GetHwnd();
	
	mHwnd = ::CreateWindowExW(
		wndExStyle,
		L"Seven Engine Window",
		Locale::ConvertStringToWstring(windowName).c_str(),
		wndStyle,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		parrentHwnd,
		nullptr,
		::GetModuleHandleW(nullptr),
		this
	);

	mWndStyle = wndStyle;
	mWndExStyle = wndExStyle;
	mImguiViewport = _viewport;

	mSwapChain = new RRenderProxy<RSwapChain>;
	mDrawData = new RRenderProxy<RImguiDrawData>;

	mSwapChainData.mWidth = rect.right - rect.left;
	mSwapChainData.mHeight = rect.bottom - rect.top;
	mSwapChainData.mRefreshRate = Math::SUIntFraction(1, 60);
	mSwapChainData.mPixelFormat = ERDIPixelFormat::R8G8B8A8_UNORM;
	mSwapChainData.mOutputWindow = mHwnd;
	mSwapChainData.mIsWindowed = true;
	mSwapChainData.mNeedResize = true;

	mIsSwapChainDiry = true;
}

void SUIInternalWindow::Release()
{
	GetRenderCommandList()->AddExpiringRenderProxy({ mSwapChain, mDrawData });
	DestroyWindow(mHwnd);

	mSwapChain = nullptr;
	mDrawData = nullptr;
	mHwnd = nullptr;
	delete this;
}

void SUIInternalWindow::ShowWindow() noexcept
{
	if (mImguiViewport->Flags & ImGuiViewportFlags_NoFocusOnAppearing)
		::ShowWindow(mHwnd, SW_SHOWNA);
	else
		::ShowWindow(mHwnd, SW_SHOW);
}

void SUIInternalWindow::SetWindowPos(Math::SFloat2 _pos) noexcept
{
	RECT rect = {
		static_cast<int>(_pos[0]),
		static_cast<int>(_pos[1]),
		static_cast<int>(_pos[0]),
		static_cast<int>(_pos[1])
	};

	::AdjustWindowRectEx(&rect, mWndStyle, FALSE, mWndExStyle);
	::SetWindowPos(mHwnd, NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

Math::SFloat2 SUIInternalWindow::GetWindowPos() noexcept
{
	POINT pos = { 0, 0 };
	::ClientToScreen(mHwnd, &pos);
	return Math::SFloat2(
		static_cast<float>(pos.x),
		static_cast<float>(pos.y)
	);
}

void SUIInternalWindow::SetWindowSize(Math::SFloat2 _size) noexcept
{
	RECT rect = {
		0,
		0,
		static_cast<int>(_size[0]),
		static_cast<int>(_size[1]),
	};

	::AdjustWindowRectEx(&rect, mWndStyle, FALSE, mWndExStyle); // Client to Screen
	::SetWindowPos(mHwnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

	mSwapChainData.mWidth = rect.right - rect.left;
	mSwapChainData.mHeight = rect.bottom - rect.top;
	mIsSwapChainDiry = true;
}

Math::SFloat2 SUIInternalWindow::GetWindowSize() noexcept
{
	RECT rect;
	::GetClientRect(mHwnd, &rect);
	return Math::SFloat2(
		static_cast<float>(rect.right - rect.left),
		static_cast<float>(rect.bottom - rect.top)
	);
}

void SUIInternalWindow::SetWindowFocus() noexcept
{
	::BringWindowToTop(mHwnd);
	::SetForegroundWindow(mHwnd);
	::SetFocus(mHwnd);
}

bool SUIInternalWindow::GetWindowFocus() noexcept
{
	return ::GetForegroundWindow() == mHwnd;
}

bool SUIInternalWindow::GetWindowMinimized() noexcept
{
	return ::IsIconic(mHwnd) != FALSE;
}

void SUIInternalWindow::SetWindowTitle(std::string_view _title) noexcept
{
	::SetWindowTextW(mHwnd, Locale::ConvertStringToWstring(_title).c_str());
}

void SUIInternalWindow::SetWindowAlpha(float _alpha) noexcept
{
	if (_alpha < 1.0f)
	{
		DWORD style = ::GetWindowLongW(mHwnd, GWL_EXSTYLE) | WS_EX_LAYERED;
		::SetWindowLongW(mHwnd, GWL_EXSTYLE, style);
		::SetLayeredWindowAttributes(mHwnd, 0, (BYTE)(255 * _alpha), LWA_ALPHA);
	}
	else
	{
		DWORD style = ::GetWindowLongW(mHwnd, GWL_EXSTYLE) & ~WS_EX_LAYERED;
		::SetWindowLongW(mHwnd, GWL_EXSTYLE, style);
	}
}

void SUIInternalWindow::FlushImguiDrawData() noexcept
{
	if (IsWindow(mHwnd) == false)
		return;

	if (mIsSwapChainDiry)
		GetRenderCommandList()->RefrashSwapChain_I(mSwapChain, mSwapChainData);

	ImDrawData* imDrawDataRaw = mImguiViewport->DrawData;
	RImguiDrawData& drawData = mDrawData->Get_GameThread();

	drawData.NewFrame();
	if (imDrawDataRaw->Valid && imDrawDataRaw->TotalVtxCount != 0)
	{
		size_t cmdCount = 0;
		for (int i = 0; i != imDrawDataRaw->CmdListsCount; ++i)
		{
			ImDrawList* drawList = imDrawDataRaw->CmdLists[i];
			cmdCount += drawList->CmdBuffer.size();
		}

		drawData.mCmdBuffer.reserve(cmdCount);
		drawData.mVertexBuffer.reserve(imDrawDataRaw->TotalVtxCount);
		drawData.mIndexBuffer.reserve(imDrawDataRaw->TotalIdxCount);

		uint32_t indexOffset = 0;
		uint32_t vertexOffset = 0;

		for (int i = 0; i != imDrawDataRaw->CmdListsCount; ++i)
		{
			ImDrawList* drawList = imDrawDataRaw->CmdLists[i];

			for (int j = 0; j != drawList->CmdBuffer.size(); ++j)
			{
				const ImDrawCmd& imCmd = drawList->CmdBuffer[j];
				RImguiDrawCmd cmd;
				cmd.mClipRect = Math::SIntRect(
					static_cast<int>(imCmd.ClipRect.x - imDrawDataRaw->DisplayPos.x),
					static_cast<int>(imCmd.ClipRect.y - imDrawDataRaw->DisplayPos.y),
					static_cast<int>(imCmd.ClipRect.z - imDrawDataRaw->DisplayPos.x),
					static_cast<int>(imCmd.ClipRect.w - imDrawDataRaw->DisplayPos.y)
				);
				cmd.mTextureId = static_cast<RRenderProxy<RImguiTexture2D>*>(imCmd.TextureId);
				cmd.mVertexOffset = imCmd.VtxOffset + vertexOffset;
				cmd.mIndexOffset = imCmd.IdxOffset + indexOffset;
				cmd.mVertexCount = imCmd.ElemCount;
				drawData.mCmdBuffer.push_back(cmd);
			}

			for (int j = 0; j != drawList->VtxBuffer.size(); ++j)
			{
				const ImDrawVert& imVert = drawList->VtxBuffer[j];
				RImguiVertex vert;
				vert.mPos = Math::SFloat2(imVert.pos.x, imVert.pos.y);
				vert.mUv = Math::SFloat2(imVert.uv.x, imVert.uv.y);
				vert.mColor = imVert.col;
				drawData.mVertexBuffer.push_back(vert);
			}

			for (int j = 0; j != drawList->IdxBuffer.size(); ++j)
			{
				drawData.mIndexBuffer.push_back(drawList->IdxBuffer[j]);
			}

			indexOffset += drawList->IdxBuffer.size();
			vertexOffset += drawList->VtxBuffer.size();
		}

		drawData.mDisplayPos = Math::SFloat2(imDrawDataRaw->DisplayPos);
		drawData.mDisplaySize = Math::SFloat2(imDrawDataRaw->DisplaySize);

		GetRenderCommandList()->RenderWindow_D(mSwapChain, mDrawData);
	}

	mIsSwapChainDiry = false;
}
