#pragma once

#include "Core/Template/TemplateUtil.h"

#include <filesystem>
#include <functional>

#include "Core/PreWindowsApi.h"
#include <d3dcompiler.h>
#include "Core/PostWindowsApi.h"

#pragma comment(lib, "d3dcompiler.lib")

enum class ED3DShaderTarget
{
	VS_5_0,
	HS_5_0,
	DS_5_0,
	GS_5_0,
	PS_5_0,
	CS_5_0,
	VS_6_0,
	HS_6_0,
	DS_6_0,
	GS_6_0,
	PS_6_0,
	CS_6_0,
	VS_6_4,
	HS_6_4,
	DS_6_4,
	GS_6_4,
	PS_6_4,
	CS_6_4,

	Num,
	Error = TErrorEnumValue<ED3DShaderTarget>
};

class D3DInclude : public ID3DInclude
{
public:
	void Init(const std::function<std::filesystem::path(const std::filesystem::path&)>& _searchIncludeFileFunc) noexcept
	{
		mSearchIncludeFileFunc = _searchIncludeFileFunc;
	}

	HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
	{
		*ppData = nullptr;
		*pBytes = 0;

		std::filesystem::path filePath = mSearchIncludeFileFunc(pFileName);

		if (std::filesystem::exists(filePath))
		{
			FILE* fp = nullptr;
			errno_t err = _wfopen_s(&fp, filePath.c_str(), L"rb");
			if (fp != nullptr)
			{
				void* fileBuffer = nullptr;
				int fileLen = 0;

				fseek(fp, 0, SEEK_END);
				fileLen = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				fileBuffer = operator new(fileLen, std::nothrow);

				if (fileBuffer == nullptr)
				{
					fclose(fp);
					return S_OK;
				}

				fread_s(fileBuffer, fileLen, fileLen, 1, fp);

				fclose(fp);

				*ppData = fileBuffer;
				*pBytes = fileLen;
			}
		}
		return S_OK;
	}

	HRESULT Close(LPCVOID pData) override
	{
		operator delete(const_cast<void*>(pData), std::nothrow);
		return S_OK;
	}

	std::function<std::filesystem::path(std::filesystem::path)> mSearchIncludeFileFunc;
};
