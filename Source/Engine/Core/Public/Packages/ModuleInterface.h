#pragma once

__interface IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;
};

struct SDefaultModule : IModuleInterface
{
	bool Init() noexcept override {}
	void Clear() noexcept override {}
};
