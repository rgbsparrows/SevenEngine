#pragma once

#include "Core/Util/UtilMacros.h"
#include "Core/Util/TemplateUtil.h"

#include <vector>
#include <stdint.h>
#include <filesystem>
#include <string_view>

struct SRDIFutureSupport;

__interface IRDIDevice;
__interface IRDIAdapter;
__interface IRDIResource;
__interface IRDICommandList;
__interface IRDICommandQueue;

__interface IRDIFactory
{
	const SRDIFutureSupport& GetFutureSupport() noexcept;
	std::string_view GetFactoryDesc() noexcept;

	bool Create() noexcept;
	void Release() noexcept;

	IRDIDevice* GetDevice() noexcept;
	const std::vector<IRDIAdapter*>& GetAdapters() noexcept;
};
