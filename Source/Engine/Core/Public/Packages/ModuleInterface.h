#pragma once

__interface IModuleInterface
{
	bool Init();
	void Clear();
};

struct SDefaultModule : IModuleInterface
{
	bool Init() override {}
	void Clear() override {}
};
