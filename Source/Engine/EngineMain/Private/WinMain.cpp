#include "EngineMain/EngineMain.h"
#include "Core/windowsEx.h"

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ wchar_t* lpCmdLine,
	_In_ int nCmdShow)
{
	hInstance;
	hPrevInstance;
	lpCmdLine;
	nCmdShow;

	SEngineMain::EngineMain();

	return 0;
}
