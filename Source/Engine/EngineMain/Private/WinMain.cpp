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

	SEngineMain::EngineMain();

	return 0;
}
