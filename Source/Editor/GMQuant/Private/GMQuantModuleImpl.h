#pragma once

#include "GMQuant/GMQuantModule.h"

class SGMQuantModuleImpl : public IGMQuantModule
{
public:
	bool Init() noexcept override;
	void Clear() noexcept override;
};

SGMQuantModuleImpl* GetGMQuantModuleImpl() noexcept;