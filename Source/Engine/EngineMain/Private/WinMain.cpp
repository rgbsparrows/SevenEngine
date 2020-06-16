#include "EngineMain/EngineMain.h"
#include "WindowsPlatform/WindowsPlatformApi.h"

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

	SEngineMain::EngineMain();

	return 0;
}
