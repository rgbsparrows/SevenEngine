#include "WindowsPlatformApi.h"
#include "EngineMain.h"

int _stdcall wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	wchar_t* lpCmdLine,
	int nCmdShow)
{
	hInstance;
	hPrevInstance;
	lpCmdLine;
	nCmdShow;
	SWindowsPlatformApi::MessageBox(nullptr, SWindowsPlatformApi::GetCommandLine(), L"", EMessageBoxFlag::MB_OK);
	SEngineMain::EngineMain();

	return 0;
}
