#pragma once

#include "RDI/RDIModule.h"

class SRDIModuleImpl : public IRDIModuleInterface
{
	bool Init() noexcept override;
	void Clear() noexcept override;

	IRDIFactory* GetRDIFactory() noexcept override { return mRdiFactory; }

private:
	IRDIFactory* mRdiFactory = nullptr;
};

SRDIModuleImpl* GetRDIModuleImpl() noexcept;
