#pragma once
#include "Packages/PackageManager.h"

#define REGIST_MODULE(_moduleClass) SPackageManager::Get().RegistModule(MAKE_WIDE(MAKE_STR(PACKAGE_NAME)), MAKE_WIDE(MAKE_STR(MODULE_NAME)), []() noexcept{return new _moduleClass;})
