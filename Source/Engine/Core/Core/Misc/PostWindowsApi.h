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
#error Î´Æ¥ÅäµÄ "PreWindowsApi.h" Óë "PostWindowsApi.h" ¶Ô
#endif

#undef _WINDOWSAPIBOUNDER_CHECK
