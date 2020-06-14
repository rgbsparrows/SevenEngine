#pragma once

#include "WindowsConstants.h"
#include <stdint.h>

__interface IUIWinMessageHandler
{
	bool ProcessMessage(EWinMessage _message, uintptr_t _wparam, intptr_t _lparam) noexcept;
};
