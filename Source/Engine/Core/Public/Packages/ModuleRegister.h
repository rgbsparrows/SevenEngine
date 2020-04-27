#pragma once
#include "Packages/PackageManager.h"

#define REGIST_MODULE(_moduleName, _moduleClass) SModuleManager::Get().RegistModule(_moduleName, []() noexcept{return new _moduleClass;})
