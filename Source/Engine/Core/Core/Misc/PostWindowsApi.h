#pragma once

#undef GetCommandLine
#undef CreateEvent
#undef CreateEventEx
#undef CreateWindow
#undef CreateWindowEx
#undef GetModuleHandle
#undef SetWindowText
#undef SetWindowLong
#undef LoadCursor
#undef DefWindowProc
#undef PeekMessage
#undef DispatchMessage
#undef max
#undef min
#undef assert

#ifndef _WINDOWSAPIBOUNDER_CHECK
#error δƥ��� "PreWindowsApi.h" �� "PostWindowsApi.h" ��
#endif

#undef _WINDOWSAPIBOUNDER_CHECK
