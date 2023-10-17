#include "UIModuleImpl.h"
#include "Core/Util/Assert.h"
#include "Core/Clock/Clock.h"
#include "Core/Misc/Localize.h"
#include "UI/WindowInterface.h"
#include "Core/Misc/windowsEx.h"
#include "Render/RenderModule.h"
#include "EngineMain/EngineMain.h"
#include "Render/RenderCommandList.h"
#include "Core/ProgramConfiguation/BasicPath.h"
#include "Core/ProgramConfiguation/BasicPath.h"
#include "Core/ProgramConfiguation/ProgramConfiguation.h"

#include "Core/Misc/PreWindowsApi.h"
#include <XInput.h>
#include "Core/Misc/PostWindowsApi.h"

#include "imgui.h"
#include "implot.h"

static SUIModuleImpl* GUIModuleImpl = nullptr;

IUIModule* GetUIModule() noexcept
{
	return GUIModuleImpl;
}

SUIModuleImpl* GetUIModuleImpl() noexcept
{
	return GUIModuleImpl;
}

REGIST_MODULE("UIModule", SUIModuleImpl)

bool SUIModuleImpl::Init() noexcept
{
	GUIModuleImpl = this;

	RegistWindowClass();

	InitImgui();

	return true;
}

void SUIModuleImpl::Clear() noexcept
{
	ClearImgui();

	UnregistWindowClass();

	GUIModuleImpl = nullptr;
}

void SUIModuleImpl::OnGUI() noexcept
{
	ProcessWndMessage();

	ImguiNewFrame();

	if (mIsMainWindowOpen)
	{
		std::string str = std::format(u8"SevenEngine FPS : {0} ###MainWindow", 1.F / SClock::Get().GetAbsoluteDeltaTime());
		ImGui::Begin(str.c_str(), &mIsMainWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MainWindow | ImGuiWindowFlags_MenuBar);
		ImGui::DockSpace(ImGui::GetID("MainDockSpace"));
		ImGui::End();
	}

	std::vector<IUIWindowInterface*> windows;

	for (auto it = mUIWindows.begin(); it != mUIWindows.end(); ++it)
		windows.push_back(it->second);

	for (auto it = mAnonymousUIWindows.begin(); it != mAnonymousUIWindows.end(); ++it)
		windows.push_back(*it);

	for (auto _window : windows)
	{
		_window->OnGui();
	}

	for (auto it = mUIWindows.begin(); it != mUIWindows.end(); void())
	{
		IUIWindowInterface* window = it->second;

		if (window->IsWindowOpen() == false)
		{
			window->Release();
			it = mUIWindows.erase(it);
		}
		else
			++it;
	}

	for (auto it = mAnonymousUIWindows.begin(); it != mAnonymousUIWindows.end(); void())
	{
		IUIWindowInterface* window = *it;

		if (window->IsWindowOpen() == false)
		{
			window->Release();
			it = mAnonymousUIWindows.erase(it);
		}
		else
			++it;
	}

	ImguiEndFrame();
}

void SUIModuleImpl::AddWindow(IUIWindowInterface* _window, const std::string& _windowTag) noexcept
{
	if (_windowTag.empty() == false)
	{
		CHECK(mUIWindows.count(_windowTag) == 0);
		mUIWindows[_windowTag] = _window;
	}
	else
	{
		mAnonymousUIWindows.push_back(_window);
	}
}

IUIWindowInterface* SUIModuleImpl::GetWindowByTag(const std::string& _windowTag) noexcept
{
	auto it = mUIWindows.find(_windowTag);
	return it == mUIWindows.end() ? nullptr : it->second;
}

void SUIModuleImpl::InitImgui() noexcept
{
	InitImguiConfig();
	InitImguiCallBack();

	mMainWindow = new SUIInternalWindow;
	mMainWindow->Init(ImGui::GetMainViewport());
	mMainWindow->ShowWindow();
	ImGui::GetMainViewport()->PlatformUserData = mMainWindow;
	ImGui::GetMainViewport()->PlatformHandle = mMainWindow->GetHwnd();
}

void SUIModuleImpl::InitImguiConfig() noexcept
{
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//ImGui::GetIO().ConfigViewportsNoAutoMerge = true;
	//ImGui::GetIO().ConfigViewportsNoTaskBarIcon = true;
	ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optImGui::GetIO()nal)
	ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor ImGui::GetIO().WantSetMousePos requests (optImGui::GetIO()nal, rarely used)
	ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;    // We can create multi-viewports on the Platform side (optImGui::GetIO()nal)
	ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // We can set ImGui::GetIO().MouseHoveredViewport correctly (optImGui::GetIO()nal, not easy)
	ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
	ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_RendererHasViewports; // We can create multi-viewports on the Renderer side (optional) // FIXME-VIEWPORT: Actually unfinished..

	ImGui::GetIO().BackendPlatformName = "Seven Engine";

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui::GetIO().Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 13.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());

	ImPlot::GetStyle().FitPadding = ImVec2(0, 0.1f);

	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Keyboard mapping. ImGui will use those indices to peek into the ImGui::GetIO().KeysDown[] array that we will update during the applicatImGui::GetIO()n lifetime.
	ImGui::GetIO().KeyMap[ImGuiKey_Tab] = VK_TAB;
	ImGui::GetIO().KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	ImGui::GetIO().KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	ImGui::GetIO().KeyMap[ImGuiKey_UpArrow] = VK_UP;
	ImGui::GetIO().KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	ImGui::GetIO().KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	ImGui::GetIO().KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	ImGui::GetIO().KeyMap[ImGuiKey_Home] = VK_HOME;
	ImGui::GetIO().KeyMap[ImGuiKey_End] = VK_END;
	ImGui::GetIO().KeyMap[ImGuiKey_Insert] = VK_INSERT;
	ImGui::GetIO().KeyMap[ImGuiKey_Delete] = VK_DELETE;
	ImGui::GetIO().KeyMap[ImGuiKey_Backspace] = VK_BACK;
	ImGui::GetIO().KeyMap[ImGuiKey_Space] = VK_SPACE;
	ImGui::GetIO().KeyMap[ImGuiKey_Enter] = VK_RETURN;
	ImGui::GetIO().KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	ImGui::GetIO().KeyMap[ImGuiKey_KeyPadEnter] = VK_RETURN;
	ImGui::GetIO().KeyMap[ImGuiKey_A] = 'A';
	ImGui::GetIO().KeyMap[ImGuiKey_C] = 'C';
	ImGui::GetIO().KeyMap[ImGuiKey_V] = 'V';
	ImGui::GetIO().KeyMap[ImGuiKey_X] = 'X';
	ImGui::GetIO().KeyMap[ImGuiKey_Y] = 'Y';
	ImGui::GetIO().KeyMap[ImGuiKey_Z] = 'Z';

	const char* xinput_dll_names[] =
	{
		"xinput1_4.dll",   // Windows 8+
		"xinput1_3.dll",   // DirectX SDK
		"xinput9_1_0.dll", // Windows Vista, Windows 7
		"xinput1_2.dll",   // DirectX SDK
		"xinput1_1.dll"    // DirectX SDK
	};

	for (int n = 0; n < IM_ARRAYSIZE(xinput_dll_names); n++)
	{
		if (HMODULE dll = ::LoadLibraryA(xinput_dll_names[n]))
		{
			mXInputDLL = dll;
			mXInputGetCapabilitiesFunc = GetProcAddress<SXInputGetCapabilitiesFuncType>(dll, "XInputGetCapabilities");
			mXInputGetStateFunc = GetProcAddress<SXInputGetStateFuncType>(dll, "XInputGetState");
			break;
		}
	}

	{
		ImGui::GetPlatformIO().Monitors.resize(0);
		::EnumDisplayMonitors(NULL, NULL, SUIModuleImpl::UpdateMonitors_EnumFunc, NULL);
	}

	{
		mFontTexture = new RRenderProxy<RTexture2D>;
		mImFontTexture = new RRenderProxy<RImguiTexture2D>;

		unsigned char* pixels;
		int width, height;
		ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		RTexture2DData texture2dData;

		SRDITexture2DResourceDesc desc;
		desc.mResourceUsage = ERDIResourceUsage::ShaderResource;
		desc.mPixelFormat = ERDIPixelFormat::R8G8B8A8_UNORM;
		desc.mSizeX = width;
		desc.mSizeY = height;

		SBlob resourceData = SBlob(pixels, 1ull * width * height * sizeof(uint32_t));
		SRange subResourceData = SRange(resourceData.GetBufferSize());

		texture2dData.mDesc = desc;
		texture2dData.mResourceData = std::move(resourceData);
		texture2dData.mSubresourceData.push_back(subResourceData);


		GetRenderCommandList()->RefrashStaticTexture2D_I(mFontTexture, std::move(texture2dData));
		GetRenderCommandList()->RefrashImTexture2D_I(mFontTexture, mImFontTexture);

		ImGui::GetIO().Fonts->TexID = mImFontTexture;
	}
}

void SUIModuleImpl::InitImguiCallBack() noexcept
{
	ImGui::GetPlatformIO().Platform_CreateWindow = [](ImGuiViewport* _viewport)
	{
		SUIInternalWindow* window = new SUIInternalWindow;
		window->Init(_viewport);
		_viewport->PlatformUserData = window;
		_viewport->PlatformHandle = window->GetHwnd();
	};

	ImGui::GetPlatformIO().Platform_DestroyWindow = [](ImGuiViewport* _viewport)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		window->Release();
		_viewport->PlatformUserData = nullptr;
		_viewport->PlatformHandle = nullptr;
	};

	ImGui::GetPlatformIO().Platform_ShowWindow = [](ImGuiViewport* _viewport)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		window->ShowWindow();
	};

	ImGui::GetPlatformIO().Platform_SetWindowPos = [](ImGuiViewport* _viewport, ImVec2 _pos)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		window->SetWindowPos(Math::SFloat2(_pos.x, _pos.y));
	};

	ImGui::GetPlatformIO().Platform_GetWindowPos = [](ImGuiViewport* _viewport)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		Math::SFloat2 pos = window->GetWindowPos();
		return ImVec2(pos[0], pos[1]);
	};

	ImGui::GetPlatformIO().Platform_SetWindowSize = [](ImGuiViewport* _viewport, ImVec2 _size)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		window->SetWindowSize(Math::SFloat2(_size.x, _size.y));
	};

	ImGui::GetPlatformIO().Platform_GetWindowSize = [](ImGuiViewport* _viewport)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		Math::SFloat2 size = window->GetWindowSize();
		return ImVec2(size[0], size[1]);
	};

	ImGui::GetPlatformIO().Platform_SetWindowFocus = [](ImGuiViewport* _viewport)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		window->SetWindowFocus();
	};

	ImGui::GetPlatformIO().Platform_GetWindowFocus = [](ImGuiViewport* _viewport)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		return window->GetWindowFocus();
	};

	ImGui::GetPlatformIO().Platform_GetWindowMinimized = [](ImGuiViewport* _viewport)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		return window->GetWindowMinimized();
	};

	ImGui::GetPlatformIO().Platform_SetWindowTitle = [](ImGuiViewport* _viewport, const char* _title)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		window->SetWindowTitle(_title);
	};

	ImGui::GetPlatformIO().Platform_SetWindowAlpha = [](ImGuiViewport* _viewport, float _alpha)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		window->SetWindowAlpha(_alpha);
	};

	ImGui::GetPlatformIO().Platform_RenderWindow = [](ImGuiViewport* _viewport, void* _renderArg)
	{
		SUIInternalWindow* window = reinterpret_cast<SUIInternalWindow*>(_viewport->PlatformUserData);
		window->FlushImguiDrawData();
	};
}

void SUIModuleImpl::ClearImgui() noexcept
{
	FreeLibrary(mXInputDLL);

	GetRenderCommandList()->AddExpiringRenderProxy({ mFontTexture, mImFontTexture });

	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void SUIModuleImpl::ProcessWndMessage() noexcept
{
	MSG msg;
	while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			SEngineMain::RequireExit();
			return;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void SUIModuleImpl::ImguiNewFrame() noexcept
{
	Math::SFloat2 mainWindowSize = mMainWindow->GetWindowSize();

	ImGui::GetIO().DisplaySize = ImVec2(mainWindowSize[0], mainWindowSize[1]);
	ImGui::GetIO().DeltaTime = SClock::Get().GetAbsoluteDeltaTime();

	// Read keyboard modifiers inputs
	ImGui::GetIO().KeyCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
	ImGui::GetIO().KeyShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
	ImGui::GetIO().KeyAlt = (::GetKeyState(VK_MENU) & 0x8000) != 0;
	ImGui::GetIO().KeySuper = false;

	if (ImGui::GetIO().WantSetMousePos)
	{
		ImVec2 pos = ImGui::GetIO().MousePos;
		::SetCursorPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
	}

	POINT mousePos;
	::GetCursorPos(&mousePos);

	ImGui::GetIO().MouseHoveredViewport = 0;
	ImGui::GetIO().MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

	if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(::WindowFromPoint(mousePos)))
		if ((viewport->Flags & ImGuiViewportFlags_NoInputs) == 0) // FIXME: We still get our NoInputs window with WM_NCHITTEST/HTTRANSPARENT code when decorated?
			ImGui::GetIO().MouseHoveredViewport = viewport->ID;

	ZeroMemory(ImGui::GetIO().NavInputs, sizeof(ImGui::GetIO().NavInputs));

	XINPUT_CAPABILITIES caps;
	bool hasGamePad = false;
	if (mXInputGetCapabilitiesFunc != nullptr)
		hasGamePad = mXInputGetCapabilitiesFunc(0, XINPUT_FLAG_GAMEPAD, &caps) == ERROR_SUCCESS;

	ImGui::GetIO().BackendFlags &= ~ImGuiBackendFlags_HasGamepad;

	XINPUT_STATE xinput_state;
	if (hasGamePad && mXInputGetStateFunc != nullptr && mXInputGetStateFunc(0, &xinput_state) == ERROR_SUCCESS)
	{
		const XINPUT_GAMEPAD& gamepad = xinput_state.Gamepad;
		ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_HasGamepad;

#define MAP_BUTTON(NAV_NO, BUTTON_ENUM)     { ImGui::GetIO().NavInputs[NAV_NO] = (gamepad.wButtons & BUTTON_ENUM) ? 1.0f : 0.0f; }
#define MAP_ANALOG(NAV_NO, VALUE, V0, V1)   { float vn = (float)(VALUE - V0) / (float)(V1 - V0); if (vn > 1.0f) vn = 1.0f; if (vn > 0.0f && ImGui::GetIO().NavInputs[NAV_NO] < vn) ImGui::GetIO().NavInputs[NAV_NO] = vn; }
		MAP_BUTTON(ImGuiNavInput_Activate, XINPUT_GAMEPAD_A);              // Cross / A
		MAP_BUTTON(ImGuiNavInput_Cancel, XINPUT_GAMEPAD_B);              // Circle / B
		MAP_BUTTON(ImGuiNavInput_Menu, XINPUT_GAMEPAD_X);              // Square / X
		MAP_BUTTON(ImGuiNavInput_Input, XINPUT_GAMEPAD_Y);              // Triangle / Y
		MAP_BUTTON(ImGuiNavInput_DpadLeft, XINPUT_GAMEPAD_DPAD_LEFT);      // D-Pad Left
		MAP_BUTTON(ImGuiNavInput_DpadRight, XINPUT_GAMEPAD_DPAD_RIGHT);     // D-Pad Right
		MAP_BUTTON(ImGuiNavInput_DpadUp, XINPUT_GAMEPAD_DPAD_UP);        // D-Pad Up
		MAP_BUTTON(ImGuiNavInput_DpadDown, XINPUT_GAMEPAD_DPAD_DOWN);      // D-Pad Down
		MAP_BUTTON(ImGuiNavInput_FocusPrev, XINPUT_GAMEPAD_LEFT_SHOULDER);  // L1 / LB
		MAP_BUTTON(ImGuiNavInput_FocusNext, XINPUT_GAMEPAD_RIGHT_SHOULDER); // R1 / RB
		MAP_BUTTON(ImGuiNavInput_TweakSlow, XINPUT_GAMEPAD_LEFT_SHOULDER);  // L1 / LB
		MAP_BUTTON(ImGuiNavInput_TweakFast, XINPUT_GAMEPAD_RIGHT_SHOULDER); // R1 / RB
		MAP_ANALOG(ImGuiNavInput_LStickLeft, gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
		MAP_ANALOG(ImGuiNavInput_LStickRight, gamepad.sThumbLX, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
		MAP_ANALOG(ImGuiNavInput_LStickUp, gamepad.sThumbLY, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
		MAP_ANALOG(ImGuiNavInput_LStickDown, gamepad.sThumbLY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32767);
#undef MAP_BUTTON
#undef MAP_ANALOG
	}

	ImGui::NewFrame();
}

void SUIModuleImpl::ImguiEndFrame() noexcept
{
	ImGui::Render();

	ImVec2 imMainWindowPos = ImGui::GetWindowPos("Seven Engine###MainWindow");
	ImVec2 imMainWindowSize = ImGui::GetWindowSize("Seven Engine###MainWindow");

	Math::SFloat2 mainWindowPos = mMainWindow->GetWindowPos();
	Math::SFloat2 mainWindowSize = mMainWindow->GetWindowSize();

	if (mainWindowPos[0] != imMainWindowPos[0] || mainWindowPos[1] != imMainWindowPos[1])
		mMainWindow->SetWindowPos(Math::SFloat2(imMainWindowPos.x, imMainWindowPos.y));
	if (mainWindowSize[0] != imMainWindowSize[0] || mainWindowSize[1] != imMainWindowSize[1])
		mMainWindow->SetWindowSize(Math::SFloat2(imMainWindowSize.x, imMainWindowSize.y));

	mMainWindow->FlushImguiDrawData();

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();

	if (mIsMainWindowOpen == false)
		SEngineMain::RequireExit();
}

void SUIModuleImpl::RegistWindowClass() noexcept
{
	std::filesystem::path iconPath;
	if (SProgramConfiguation::IsWithProject())
		iconPath = SBasicPath::GetProjectPath() / "Game.ico";

	if (std::filesystem::exists(iconPath) == false)
		iconPath = SBasicPath::GetEnginePath() / "RgbSparrows.ico";

	HANDLE icon = LoadImageW(::GetModuleHandleW(nullptr), iconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	mIcon = static_cast<HICON>(icon);

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &SUIModuleImpl::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = ::GetModuleHandleW(nullptr);
	wcex.hIcon = mIcon;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Seven Engine Window";
	wcex.hIconSm = mIcon;

	::RegisterClassExW(&wcex);
}

void SUIModuleImpl::UnregistWindowClass() noexcept
{
	::UnregisterClassW(L"Seven Engine Window", ::GetModuleHandleW(nullptr));
	DestroyIcon(mIcon);
	mIcon = nullptr;
}

LRESULT SUIModuleImpl::WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (ImGui::GetCurrentContext() == NULL)
		return ::DefWindowProcW(_hwnd, _msg, _wParam, _lParam);

	ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(_hwnd);
	if (viewport == nullptr)
		return ::DefWindowProcW(_hwnd, _msg, _wParam, _lParam);

	SUIInternalWindow* uiWindow = static_cast<SUIInternalWindow*>(viewport->PlatformUserData);

	switch (_msg)
	{
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
	{
		int button = 0;
		if (_msg == WM_LBUTTONDOWN || _msg == WM_LBUTTONDBLCLK) { button = 0; }
		if (_msg == WM_RBUTTONDOWN || _msg == WM_RBUTTONDBLCLK) { button = 1; }
		if (_msg == WM_MBUTTONDOWN || _msg == WM_MBUTTONDBLCLK) { button = 2; }
		if (_msg == WM_XBUTTONDOWN || _msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(_wParam) == XBUTTON1) ? 3 : 4; }
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
			::SetCapture(_hwnd);
		ImGui::GetIO().MouseDown[button] = true;
		return 0;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	{
		int button = 0;
		if (_msg == WM_LBUTTONUP) { button = 0; }
		if (_msg == WM_RBUTTONUP) { button = 1; }
		if (_msg == WM_MBUTTONUP) { button = 2; }
		if (_msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(_wParam) == XBUTTON1) ? 3 : 4; }
		ImGui::GetIO().MouseDown[button] = false;
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == _hwnd)
			::ReleaseCapture();
		break;
	}
	case WM_MOUSEWHEEL:
		ImGui::GetIO().MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(_wParam) / (float)WHEEL_DELTA;
		break;
	case WM_MOUSEHWHEEL:
		ImGui::GetIO().MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(_wParam) / (float)WHEEL_DELTA;
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (_wParam < 256)
			ImGui::GetIO().KeysDown[_wParam] = 1;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (_wParam < 256)
			ImGui::GetIO().KeysDown[_wParam] = 0;
		break;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (_wParam > 0 && _wParam < 0x10000)
			ImGui::GetIO().AddInputCharacterUTF16((unsigned short)_wParam);
		break;
	case WM_SETCURSOR:
		if (LOWORD(_lParam) == HTCLIENT && ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0) != 0)
		{
			LPSTR win32_cursor = nullptr;
			switch (ImGui::GetMouseCursor())
			{
			case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
			case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
			case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
			case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
			case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
			case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
			case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
			case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
			case ImGuiMouseCursor_NotAllowed:   win32_cursor = IDC_NO; break;
			case ImGuiMouseCursor_None:			win32_cursor = nullptr; break;
			}

			HCURSOR cursor = nullptr;
			if (win32_cursor != nullptr)
				cursor = ::LoadCursorA(nullptr, win32_cursor);

			::SetCursor(cursor);

			return TRUE;
		}
		break;
	case WM_CLOSE:
		viewport->PlatformRequestClose = true;
		break;
	case WM_MOVE:
		viewport->PlatformRequestMove = true;
		break;
	case WM_SIZE:
		viewport->PlatformRequestResize = true;
		break;
	case WM_MOUSEACTIVATE:
		if (viewport->Flags & ImGuiViewportFlags_NoFocusOnClick)
			return MA_NOACTIVATE;
		break;
	case WM_SYSCOMMAND:
		if ((_wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		if (viewport == ImGui::GetMainViewport())
		{
			SEngineMain::RequireExit();
			return 0;
		}
	}

	return ::DefWindowProcW(_hwnd, _msg, _wParam, _lParam);
}

BOOL SUIModuleImpl::UpdateMonitors_EnumFunc(HMONITOR _monitor, HDC, LPRECT, LPARAM)
{
	static HMODULE shcoreDll = ::LoadLibraryA("shcore.dll");
	static SGetDpiForMonitorFuncType* mGetDpiForMonitorFunc = GetProcAddress<SGetDpiForMonitorFuncType>(shcoreDll, "GetDpiForMonitor");

	static std::unique_ptr<HMODULE, void(*)(HMODULE* dllHandle)> shcoreDllDeleter = std::unique_ptr<HMODULE, void(*)(HMODULE* dllHandle)>(&shcoreDll, [](HMODULE* dllHandle) { FreeLibrary(*dllHandle); });

	UINT xdpi = 96, ydpi = 96;
	mGetDpiForMonitorFunc((HMONITOR)_monitor, MDT_EFFECTIVE_DPI, &xdpi, &ydpi);
	CHECK(xdpi == ydpi);

	MONITORINFO info = {};
	info.cbSize = sizeof(MONITORINFO);
	if (!::GetMonitorInfo(_monitor, &info))
		return TRUE;
	ImGuiPlatformMonitor imgui_monitor;
	imgui_monitor.MainPos = ImVec2((float)info.rcMonitor.left, (float)info.rcMonitor.top);
	imgui_monitor.MainSize = ImVec2((float)(info.rcMonitor.right - info.rcMonitor.left), (float)(info.rcMonitor.bottom - info.rcMonitor.top));
	imgui_monitor.WorkPos = ImVec2((float)info.rcWork.left, (float)info.rcWork.top);
	imgui_monitor.WorkSize = ImVec2((float)(info.rcWork.right - info.rcWork.left), (float)(info.rcWork.bottom - info.rcWork.top));
	imgui_monitor.DpiScale = xdpi / 96.f;
	ImGuiPlatformIO& io = ImGui::GetPlatformIO();
	if (info.dwFlags & MONITORINFOF_PRIMARY)
		io.Monitors.push_front(imgui_monitor);
	else
		io.Monitors.push_back(imgui_monitor);
	return TRUE;
}
